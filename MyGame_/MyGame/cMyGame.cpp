// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/CollisionSystem/BoxCollisionComponent.h>
#include <Engine/CollisionSystem/SphereCollisionComponent.h>
#include <random>
#include <thread> // For sleep_for
#include <chrono> // For duration

// Inherited Implementation
//=========================

// Run
//----

/*
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
*/


// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	m_playerInput = new PlayerInput();
	m_collisionManager = Collision::CollisionManager::GetCollisionManager();
	m_collisionManager->Begin();

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
	m_Camera->m_RigidBodyState.position = Math::sVector(0.0f, 1.0f, 5.0f);
	//m_Camera->m_cameraPosition = m_Camera->m_RigidBodyState.position;
	//m_Camera->m_RigidBodyState.velocity = Math::sVector(-0.1f, 0.0f, 0.0f);

	// game object
	m_playerGameObject = new Assets::sGameObject();
	m_playerGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_playerGameObject->m_RigidBodyState.position = Math::sVector(0.0f, 0.3f, 0.0f);
	m_playerGameObject->m_collisionComp = new Collision::BoxCollisionComponent();
	m_playerGameObject->m_collisionComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);
	static_cast<Collision::BoxCollisionComponent*>(m_playerGameObject->m_collisionComp)->SetExtend(Math::sVector(0.15f, 0.15f, 0.15f));
	m_playerGameObject->m_collisionComp->SetPosition(m_playerGameObject->m_RigidBodyState.position);
	m_collisionManager->AddCollisionComponent(*(m_playerGameObject->m_collisionComp));

	m_coinGameObject = new Assets::sGameObject();
	m_coinGameObject->m_RigidBodyState = Physics::sRigidBodyState();
	m_coinGameObject->m_RigidBodyState.position = Math::sVector(0.0f, 1.5f + m_enemyCount * 2.5f, 0.0f);
	m_coinGameObject->m_RigidBodyState.velocity = Math::sVector(0.0f, -0.3f, 0.0f);
	m_coinGameObject->m_collisionComp = new Collision::SphereCollisionComponent();
	m_coinGameObject->m_collisionComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);
	static_cast<Collision::SphereCollisionComponent*>(m_coinGameObject->m_collisionComp)->SetRadius(0.2f);
	m_coinGameObject->m_collisionComp->SetPosition(m_coinGameObject->m_RigidBodyState.position);
	m_collisionManager->AddCollisionComponent(*(m_coinGameObject->m_collisionComp));

	m_enemyGameObjectList = new Assets::sGameObject * [m_enemyCount];

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
		m_playerGameObject->m_Effect = m_myShaderEffect;
		m_coinGameObject->m_Effect = m_standardShaderEffect;
	}
	// Initialize the geometry
	{


		if (!(result = Graphics::cMesh::Load("player", m_playerMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = Graphics::cMesh::Load("enemy", m_enemyMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = Graphics::cMesh::Load("coin", m_coinMesh)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		m_playerGameObject->m_Mesh = m_playerMesh;
		m_coinGameObject->m_Mesh = m_coinMesh;
	}

	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<float> dist(-1.5f, 1.5f);

	for (int i = 0; i < m_enemyCount; i++) {
		Assets::sGameObject* i_enemy = new Assets::sGameObject();
		i_enemy->m_RigidBodyState = Physics::sRigidBodyState();
		float i_rand_x = dist(gen);
		float i_rand_z = dist(gen);
		i_enemy->m_RigidBodyState.position = Math::sVector(i_rand_x, 2.5f + i * 2.5f, i_rand_z);
		i_enemy->m_RigidBodyState.velocity = Math::sVector(0.0f, -0.3f, 0.0f);
		i_enemy->m_Effect = m_standardShaderEffect;
		i_enemy->m_Mesh = m_enemyMesh;
		i_enemy->m_collisionComp = new Collision::SphereCollisionComponent();
		i_enemy->m_collisionComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);
		static_cast<Collision::SphereCollisionComponent*>(i_enemy->m_collisionComp)->SetRadius(0.8f);
		i_enemy->m_collisionComp->SetPosition(i_enemy->m_RigidBodyState.position);
		m_collisionManager->AddCollisionComponent(*(i_enemy->m_collisionComp));
		m_enemyGameObjectList[i] = i_enemy;
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

	if (m_playerMesh)
	{
		m_playerMesh->DecrementReferenceCount();
		m_playerMesh = nullptr;
	}

	if (m_enemyMesh)
	{
		m_enemyMesh->DecrementReferenceCount();
		m_enemyMesh = nullptr;
	}

	if (m_coinMesh)
	{
		m_coinMesh->DecrementReferenceCount();
		m_coinMesh = nullptr;
	}

	for (int i = 0; i < m_pgsMeshes->m_staticMeshCount; i++) {
		if (m_pgsMeshes->m_staticMeshArr[i]) {
			m_pgsMeshes->m_staticMeshArr[i]->DecrementReferenceCount();
			m_pgsMeshes->m_staticMeshArr[i] = nullptr;
		}
	}

	delete m_playerGameObject;
	delete m_coinGameObject;
	for (int i = 0; i < m_enemyCount; i++) {
		delete m_enemyGameObjectList[i];
	}
	delete m_pgsMeshes;

	m_collisionManager->Destroy();

	Logging::OutputMessage("Junxuan-Hu's Game Cleaning Up!");
	return Results::Success;
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	if (m_collisionTime > 0) {
		if (GetElapsedSecondCount_simulation() - m_collisionTime >= 3.0f) {
			const auto result = Exit(EXIT_SUCCESS);
			EAE6320_ASSERT(result);
		}
		else {
			return;
		}
	}

	PlayerInput::InputState i_inputState = PlayerInput::InputState();
	m_playerInput->Update(i_inputState);
	if (i_inputState.quitGame) {
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
	m_playerGameObject->m_RigidBodyState.velocity = i_inputState.objectMoveDirection * 0.5f;
	m_Camera->m_RigidBodyState.velocity = i_inputState.cameraMoveDirection * 0.5f;

	m_Camera->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);   
	m_playerGameObject->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_coinGameObject->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);

	for (int i = 0; i < m_enemyCount; i++) {
		m_enemyGameObjectList[i]->m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	}

	m_playerGameObject->m_collisionComp->SetPosition(m_playerGameObject->m_RigidBodyState.position);
	m_coinGameObject->m_collisionComp->SetPosition(m_coinGameObject->m_RigidBodyState.position);
	for (int i = 0; i < m_enemyCount; i++) {
		m_enemyGameObjectList[i]->m_collisionComp->SetPosition(m_enemyGameObjectList[i]->m_RigidBodyState.position);
	}

	m_collisionManager->Update();
	for (int i = 0; i < m_enemyCount; i++) {
		if (m_enemyGameObjectList[i]->m_collisionComp->DetectCollision(*(m_playerGameObject->m_collisionComp))) {
			m_enemyGameObjectList[i]->m_RigidBodyState.velocity = Math::sVector(0.0f, 0.0f, 0.0f);
			m_backgroundColor[0] = 0.0f;
			m_backgroundColor[1] = 0.0f;
			m_backgroundColor[2] = 0.0f;
			m_collisionTime = GetElapsedSecondCount_simulation();
		}
	}
	if (m_coinGameObject->m_collisionComp->DetectCollision(*(m_playerGameObject->m_collisionComp))) {
		m_coinGameObject->m_RigidBodyState.velocity = Math::sVector(0.0f, 0.0f, 0.0f);
		m_backgroundColor[0] = 1.0f;
		m_backgroundColor[1] = 1.0f;
		m_backgroundColor[2] = 1.0f;
		m_collisionTime = GetElapsedSecondCount_simulation();
	}

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
	Assets::sGameObject** i_gameObjectsToSubmit = new Assets::sGameObject * [2 + m_enemyCount];
	i_gameObjectsToSubmit[0] = m_playerGameObject;
	i_gameObjectsToSubmit[1] = m_coinGameObject;
	for (int i = 0; i < m_enemyCount; i++) {
		i_gameObjectsToSubmit[i + 2] = m_enemyGameObjectList[i];
	}
	SubmitGameObjectsRenderData(m_Camera, m_backgroundColor, 2 + m_enemyCount, i_gameObjectsToSubmit, i_elapsedSecondCount_sinceLastSimulationUpdate);

}
