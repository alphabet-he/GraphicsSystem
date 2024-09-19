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
			eae6320::Graphics::cEffect* i_effect = new eae6320::Graphics::cEffect("standard", "myshader");
			if (!(result = i_effect->InitializeShadingData()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
			m_EffectReleaseToShow = i_effect;
		}

		// press-to-show effect
		{
			eae6320::Graphics::cEffect* i_effect = new eae6320::Graphics::cEffect("standard", "standard");
			if (!(result = i_effect->InitializeShadingData()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
			m_EffectPressToShow = i_effect;
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

			eae6320::Graphics::cMesh* i_mesh = new eae6320::Graphics::cMesh(static_cast<unsigned int>(2), static_cast<unsigned int>(3), i_vertexData, i_indices);
			if (!(result = i_mesh->InitializeGeometry()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
			m_MeshReleaseToShow = i_mesh;
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

			eae6320::Graphics::cMesh* i_mesh = new eae6320::Graphics::cMesh(static_cast<unsigned int>(1), static_cast<unsigned int>(3), i_vertexData, i_indices);
			if (!(result = i_mesh->InitializeGeometry()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
			m_MeshPressToShow = i_mesh;
		}

	}
	
	Logging::OutputMessage("Junxuan-Hu's Game Initializd!");

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	Logging::OutputMessage("Junxuan-Hu's Game Cleaning Up!");
	return Results::Success;
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::cMesh** i_meshToSubmit = new eae6320::Graphics::cMesh*[2];
	i_meshToSubmit[0] = m_MeshReleaseToShow;
	i_meshToSubmit[1] = m_MeshPressToShow;

	eae6320::Graphics::cEffect** i_effectToSubmit = new eae6320::Graphics::cEffect*[2];
	i_effectToSubmit[0] = m_EffectReleaseToShow;
	i_effectToSubmit[1] = m_EffectPressToShow;

	Graphics::SubmitRenderData(m_backgroundColor, 2, i_meshToSubmit, i_effectToSubmit);
}
