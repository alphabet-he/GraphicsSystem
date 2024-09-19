// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>

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

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Shift))
	{
		// not show m_MeshReleaseToShow
		m_shiftKeyPressed = true;
	}
	else {
		m_shiftKeyPressed = false;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		// not show m_MeshReleaseToShow
		m_spaceKeyPressed = true;
	}
	else
	{
		m_spaceKeyPressed = false;
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{

	auto result = Results::Success;

	// background co
	m_backgroundColor[0] = 1.0f;
	m_backgroundColor[1] = 0.0f;
	m_backgroundColor[2] = 1.0f;

	// Initialize the shading data
	{

		// release-to-show effect
		{
			if (!(result = Graphics::cEffect::Load("standard", "myshader", m_EffectReleaseToShow)))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
		}

		// press-to-show effect
		{
			if (!(result = Graphics::cEffect::Load("standard", "standard", m_EffectPressToShow)))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
		}
	}
	// Initialize the geometry
	{

		// release-to-show mesh
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

			if (!(result = Graphics::cMesh::Load(static_cast<unsigned int>(2), static_cast<unsigned int>(3), i_vertexData, i_indices, m_MeshReleaseToShow)))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
		}

		// press-to-show mesh
		{
			eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[3];
			{
				// clockwise
				i_vertexData[0].x = 0.0f;
				i_vertexData[0].y = 0.0f;
				i_vertexData[0].z = 0.0f;

				i_vertexData[1].x = -1.5f;
				i_vertexData[1].y = 1.0f;
				i_vertexData[1].z = 0.0f;

				i_vertexData[2].x = -1.5f;
				i_vertexData[2].y = 0.0f;
				i_vertexData[2].z = 0.0f;
			}

			uint16_t* i_indices = new uint16_t[3]{ 0, 1, 2 };

			if (!(result = Graphics::cMesh::Load(static_cast<unsigned int>(1), static_cast<unsigned int>(3), i_vertexData, i_indices, m_MeshPressToShow)))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
		}

	}
	
	Logging::OutputMessage("Junxuan-Hu's Game Initializd!");

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	if (m_EffectReleaseToShow)
	{
		m_EffectReleaseToShow->DecrementReferenceCount();
		m_EffectReleaseToShow = nullptr;
	}

	if (m_EffectPressToShow)
	{
		m_EffectPressToShow->DecrementReferenceCount();
		m_EffectPressToShow = nullptr;
	}

	if (m_MeshReleaseToShow)
	{
		m_MeshReleaseToShow->DecrementReferenceCount();
		m_MeshReleaseToShow = nullptr;
	}

	if (m_MeshPressToShow)
	{
		m_MeshPressToShow->DecrementReferenceCount();
		m_MeshPressToShow = nullptr;
	}

	Logging::OutputMessage("Junxuan-Hu's Game Cleaning Up!");
	return Results::Success;
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::cMesh** i_meshToSubmit;
	eae6320::Graphics::cEffect** i_effectToSubmit;
	uint16_t i_count = 0;

	// situations with key pressed or not
	{
		// press space, release shift: show two meshes
		if (!m_shiftKeyPressed && m_spaceKeyPressed) {
			i_meshToSubmit = new eae6320::Graphics::cMesh * [2];
			i_meshToSubmit[0] = m_MeshReleaseToShow;
			i_meshToSubmit[1] = m_MeshPressToShow;

			i_effectToSubmit = new eae6320::Graphics::cEffect * [2];
			i_effectToSubmit[0] = m_EffectReleaseToShow;
			i_effectToSubmit[1] = m_EffectPressToShow;

			i_count = 2;
		}

		// press space, press shift: show press-to-show mesh
		if (m_shiftKeyPressed && m_spaceKeyPressed) {
			i_meshToSubmit = new eae6320::Graphics::cMesh * [1];
			i_meshToSubmit[0] = m_MeshPressToShow;

			i_effectToSubmit = new eae6320::Graphics::cEffect * [1];
			i_effectToSubmit[0] = m_EffectPressToShow;

			i_count = 1;
		}

		// release space, release shift: show release-to-show mesh
		if (!m_shiftKeyPressed && !m_spaceKeyPressed) {
			i_meshToSubmit = new eae6320::Graphics::cMesh * [1];
			i_meshToSubmit[0] = m_MeshReleaseToShow;

			i_effectToSubmit = new eae6320::Graphics::cEffect * [1];
			i_effectToSubmit[0] = m_EffectReleaseToShow;

			i_count = 1;
		}

		// release space, press shift: show no mesh
		if (m_shiftKeyPressed && !m_spaceKeyPressed) {
			i_meshToSubmit = nullptr;
			i_effectToSubmit = nullptr;

			i_count = 0;
		}
	}


	Graphics::SubmitRenderData(m_backgroundColor, i_count, i_meshToSubmit, i_effectToSubmit);

}
