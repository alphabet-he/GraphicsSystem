// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/cMatrix_transformation.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	// Move camera
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
		{
			m_Camera->m_RigidBodyState.velocity.x = -0.5f;
		}
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right)) {
			m_Camera->m_RigidBodyState.velocity.x = 0.5f;
		}
		else {
			m_Camera->m_RigidBodyState.velocity.x = 0.0f;
		}

		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
		{
			m_Camera->m_RigidBodyState.velocity.y = -0.5f;
		}
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up)) {
			m_Camera->m_RigidBodyState.velocity.y = 0.5f;
		}
		else {
			m_Camera->m_RigidBodyState.velocity.y = 0.0f;
		}

		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Control))
		{
			m_Camera->m_RigidBodyState.velocity.z = -0.5f;
		}
		else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Shift)) {
			m_Camera->m_RigidBodyState.velocity.z = 0.5f;
		}
		else {
			m_Camera->m_RigidBodyState.velocity.z = 0.0f;
		}
	}

	// Move game object
	{
		if (UserInput::IsKeyPressed('A'))
		{
			m_coneGameObject->m_RigidBodyState.velocity.x = -0.5f;
		}
		else if (UserInput::IsKeyPressed('D')) {
			m_coneGameObject->m_RigidBodyState.velocity.x = 0.5f;
		}
		else {
			m_coneGameObject->m_RigidBodyState.velocity.x = 0.0f;
		}

		if (UserInput::IsKeyPressed('S'))
		{
			m_coneGameObject->m_RigidBodyState.velocity.y = -0.5f;
		}
		else if (UserInput::IsKeyPressed('W')) {
			m_coneGameObject->m_RigidBodyState.velocity.y = 0.5f;
		}
		else {
			m_coneGameObject->m_RigidBodyState.velocity.y = 0.0f;
		}
	}

}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{

	auto result = Results::Success;

	// background color
	m_backgroundColor[0] = 1.0f;
	m_backgroundColor[1] = 0.0f;
	m_backgroundColor[2] = 1.0f;

	// camera
	m_Camera = new Graphics::sCamera();
	m_Camera->m_verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(45.0f);
	m_Camera->m_z_nearPlane = 0.1f;
	m_Camera->m_z_farPlane = 10.0f;
	m_Camera->m_RigidBodyState = Physics::sRigidBodyState();
	m_Camera->m_RigidBodyState.position = Math::sVector(0.0f, 0.0f, 5.0f);
	//m_Camera->m_cameraPosition = m_Camera->m_RigidBodyState.position;
	//m_Camera->m_RigidBodyState.velocity = Math::sVector(-0.1f, 0.0f, 0.0f);

	// game object
	m_planeGameObject = new Assets::sGameObject();
	m_planeGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_coneGameObject = new Assets::sGameObject();
	m_coneGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_torusGameObject = new Assets::sGameObject();
	m_torusGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_helixGameObject = new Assets::sGameObject();
	m_helixGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	//m_gameObject->m_RigidBodyState.velocity = Math::sVector(-0.1f, 0.0f, 0.0f);

	// Initialize the shading data
	{
		if (!(result = Graphics::cEffect::Load("standard", "myshader", m_myShaderEffect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		if (!(result = Graphics::cEffect::Load("standard", "standard", m_standardShaderEffect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		m_planeGameObject->m_Effect = m_standardShaderEffect;
		m_torusGameObject->m_Effect = m_standardShaderEffect;
		m_coneGameObject->m_Effect = m_myShaderEffect;
		m_helixGameObject->m_Effect = m_standardShaderEffect;
	}
	// Initialize the geometry
	{

		if (!(result = Graphics::cMesh::Load("plane", m_planeMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = Graphics::cMesh::Load("cone", m_coneMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = Graphics::cMesh::Load("torus", m_torusMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
		
		if (!(result = Graphics::cMesh::Load("helix", m_helixMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		m_planeGameObject->m_Mesh = m_planeMesh;
		m_coneGameObject->m_Mesh = m_coneMesh;
		m_torusGameObject->m_Mesh = m_torusMesh;
		m_helixGameObject->m_Mesh = m_helixMesh;
	}
	
	
	Logging::OutputMessage("Junxuan-Hu's Game Initializd!");

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	if (m_myShaderEffect)
	{
		m_myShaderEffect->DecrementReferenceCount();
		m_myShaderEffect = nullptr;
	}

	if (m_standardShaderEffect)
	{
		m_standardShaderEffect->DecrementReferenceCount();
		m_standardShaderEffect = nullptr;
	}

	if (m_planeMesh)
	{
		m_planeMesh->DecrementReferenceCount();
		m_planeMesh = nullptr;
	}

	if (m_coneMesh)
	{
		m_coneMesh->DecrementReferenceCount();
		m_coneMesh = nullptr;
	}

	if (m_torusMesh)
	{
		m_torusMesh->DecrementReferenceCount();
		m_torusMesh = nullptr;
	}

	if (m_helixMesh)
	{
		m_helixMesh->DecrementReferenceCount();
		m_helixMesh = nullptr;
	}


	delete m_planeGameObject;
	delete m_coneGameObject;
	delete m_torusGameObject;
	delete m_helixGameObject;

	Logging::OutputMessage("Junxuan-Hu's Game Cleaning Up!");
	return Results::Success;
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_Camera->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_coneGameObject->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cMyGame::SubmitGameObjectsRenderData
(
	Graphics::sCamera* i_camera,
	float i_background[],
	uint16_t i_cnt, Assets::sGameObject** i_gameObjects,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate
)
{
	Graphics::cMesh** i_meshArrToSubmit = new Graphics::cMesh * [i_cnt];
	Graphics::cEffect** i_effectArrToSubmit = new Graphics::cEffect * [i_cnt];
	Math::cMatrix_transformation* i_localToWorldMatrixArrToSubmit = new Math::cMatrix_transformation[i_cnt];

	for (int i = 0; i < i_cnt; i++) {
		i_meshArrToSubmit[i] = i_gameObjects[i]->m_Mesh;
		i_effectArrToSubmit[i] = i_gameObjects[i]->m_Effect;
		i_localToWorldMatrixArrToSubmit[i] = i_gameObjects[i]->m_RigidBodyState.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}

	m_Camera->m_cameraPositionPredicted = m_Camera->m_RigidBodyState.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);

	Graphics::SubmitRenderData(i_camera, i_background,
		i_cnt, i_meshArrToSubmit, i_effectArrToSubmit, i_localToWorldMatrixArrToSubmit);
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Assets::sGameObject** i_gameObjectsToSubmit = new Assets::sGameObject * [4];
	i_gameObjectsToSubmit[0] = m_planeGameObject;
	i_gameObjectsToSubmit[1] = m_coneGameObject;
	i_gameObjectsToSubmit[2] = m_torusGameObject;
	i_gameObjectsToSubmit[3] = m_helixGameObject;
	SubmitGameObjectsRenderData(m_Camera, m_backgroundColor, 4, i_gameObjectsToSubmit, i_elapsedSecondCount_sinceLastSimulationUpdate);

}
