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

		if (UserInput::IsKeyPressed('J'))
		{
			m_Camera->m_RigidBodyState.angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 1.0f);
			m_Camera->m_RigidBodyState.angularSpeed = -0.1f;
		}
		else if (UserInput::IsKeyPressed('L')) {
			m_Camera->m_RigidBodyState.angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 1.0f);
			m_Camera->m_RigidBodyState.angularSpeed = 0.1f;
		}
		else {
			m_Camera->m_RigidBodyState.angularSpeed = 0.0f;
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
	m_coneGameObject = new Assets::sGameObject();
	m_coneGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_torusGameObject = new Assets::sGameObject();
	m_torusGameObject->m_RigidBodyState = Physics::sRigidBodyState();
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

		m_torusGameObject->m_Effect = m_standardShaderEffect;
		m_coneGameObject->m_Effect = m_myShaderEffect;
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

		m_coneGameObject->m_Mesh = m_coneMesh;
		m_torusGameObject->m_Mesh = m_torusMesh;
	}
	
	// when game starts, submit the static objects
	//Graphics::cEffect** i_staticEffectToSubmit = new Graphics::cEffect * [2];
	//i_staticEffectToSubmit[0] = m_standardShaderEffect;
	//i_staticEffectToSubmit[1] = m_standardShaderEffect;
	//Graphics::cMesh** i_staticMeshToSubmit = new Graphics::cMesh * [2];
	//i_staticMeshToSubmit[0] = m_planeMesh;
	//i_staticMeshToSubmit[1] = m_helixMesh;

	m_pgsMeshes = new Assets::sProceduralGeneratedMesh();
	Assets::sProceduralGeneratedMesh::Load("terrain", m_pgsMeshes);
	Graphics::cEffect** i_staticEffectToSubmit = new Graphics::cEffect * [m_pgsMeshes->m_staticMeshCount];
	for (int i = 0; i < m_pgsMeshes->m_staticMeshCount; i++) {
		i_staticEffectToSubmit[i] = m_standardShaderEffect;
	}

	Graphics::SubmitStaticObjectData(m_pgsMeshes->m_staticMeshCount, m_pgsMeshes->m_staticMeshArr, i_staticEffectToSubmit);
	//Graphics::SubmitStaticObjectData(1, i_staticMeshToSubmit, i_staticEffectToSubmit);

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

	for (int i = 0; i < m_pgsMeshes->m_staticMeshCount; i++) {
		if (m_pgsMeshes->m_staticMeshArr[i]) {
			m_pgsMeshes->m_staticMeshArr[i]->DecrementReferenceCount();
			m_pgsMeshes->m_staticMeshArr[i] = nullptr;
		}
	}

	delete m_coneGameObject;
	delete m_torusGameObject;
	delete m_pgsMeshes;

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
	m_Camera->m_cameraOrientationPredicted = m_Camera->m_RigidBodyState.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate);

	Graphics::SubmitRenderData(i_camera, i_background,
		i_cnt, i_meshArrToSubmit, i_effectArrToSubmit, i_localToWorldMatrixArrToSubmit);
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Assets::sGameObject** i_gameObjectsToSubmit = new Assets::sGameObject * [2];
	i_gameObjectsToSubmit[0] = m_coneGameObject;
	i_gameObjectsToSubmit[1] = m_torusGameObject;
	SubmitGameObjectsRenderData(m_Camera, m_backgroundColor, 2, i_gameObjectsToSubmit, i_elapsedSecondCount_sinceLastSimulationUpdate);

}
