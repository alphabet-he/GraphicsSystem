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
			m_gameObject->m_RigidBodyState.velocity.x = -0.5f;
		}
		else if (UserInput::IsKeyPressed('D')) {
			m_gameObject->m_RigidBodyState.velocity.x = 0.5f;
		}
		else {
			m_gameObject->m_RigidBodyState.velocity.x = 0.0f;
		}

		if (UserInput::IsKeyPressed('S'))
		{
			m_gameObject->m_RigidBodyState.velocity.y = -0.5f;
		}
		else if (UserInput::IsKeyPressed('W')) {
			m_gameObject->m_RigidBodyState.velocity.y = 0.5f;
		}
		else {
			m_gameObject->m_RigidBodyState.velocity.y = 0.0f;
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
	m_Camera->m_RigidBodyState.position = Math::sVector(0.0f, 0.0f, 8.0f);
	//m_Camera->m_cameraPosition = m_Camera->m_RigidBodyState.position;
	//m_Camera->m_RigidBodyState.velocity = Math::sVector(-0.1f, 0.0f, 0.0f);

	// game object
	m_gameObject = new Assets::sGameObject();
	m_gameObject->m_RigidBodyState = Physics::sRigidBodyState();
	//m_gameObject->m_RigidBodyState.velocity = Math::sVector(-0.1f, 0.0f, 0.0f);

	// Initialize the shading data
	{
		Graphics::cEffect* i_effect;
		if (!(result = Graphics::cEffect::Load("standard", "myshader", i_effect)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		m_gameObject->m_Effect = i_effect;

	}
	// Initialize the geometry
	{
		eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[6];
		{
			// clockwise
			i_vertexData[0].x = 0.0f;
			i_vertexData[0].y = 0.0f;
			i_vertexData[0].z = 0.0f;

			i_vertexData[1].x = 1.0f;
			i_vertexData[1].y = 0.0f;
			i_vertexData[1].z = 0.0f;

			i_vertexData[2].x = 1.0f;
			i_vertexData[2].y = 1.0f;
			i_vertexData[2].z = 0.0f;

			i_vertexData[3].x = 0.0f;
			i_vertexData[3].y = 0.0f;
			i_vertexData[3].z = 0.0f;

			i_vertexData[4].x = 1.0f;
			i_vertexData[4].y = 1.0f;
			i_vertexData[4].z = 0.0f;

			i_vertexData[5].x = 0.0f;
			i_vertexData[5].y = 1.0f;
			i_vertexData[5].z = 0.0f;
		}

		uint16_t* i_indices = new uint16_t[6]{ 0, 1, 2, 3, 4, 5 };

		Graphics::cMesh* i_mesh;
		if (!(result = Graphics::cMesh::Load(static_cast<unsigned int>(2), static_cast<unsigned int>(3), i_vertexData, i_indices, i_mesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
		m_gameObject->m_Mesh = i_mesh;
	}
	
	
	Logging::OutputMessage("Junxuan-Hu's Game Initializd!");

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	delete m_gameObject;

	Logging::OutputMessage("Junxuan-Hu's Game Cleaning Up!");
	return Results::Success;
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_Camera->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObject->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
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
	Assets::sGameObject** i_gameObjectsToSubmit = new Assets::sGameObject * [1];
	i_gameObjectsToSubmit[0] = m_gameObject;
	SubmitGameObjectsRenderData(m_Camera, m_backgroundColor, 1, i_gameObjectsToSubmit, i_elapsedSecondCount_sinceLastSimulationUpdate);

}
