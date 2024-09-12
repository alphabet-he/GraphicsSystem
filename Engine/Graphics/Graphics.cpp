// Includes
//=========

#include "Graphics.h"

#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cShader.h"
#include "cVertexFormat.h"
#include "sContext.h"
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <new>
#include <utility>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cView.h>

// Static Data
//============

namespace
{
	// In Direct3D "views" are objects that allow a texture to be used a particular way:
	eae6320::Graphics::cView* s_View = nullptr;

	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_frame(eae6320::Graphics::ConstantBufferTypes::Frame);

	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
	};
	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];
	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;

	// Geometry Data
	//--------------
	eae6320::Graphics::cMesh** s_meshArr = nullptr;
	uint16_t s_meshCount = 0;

	// Shading Data
	//-------------
	eae6320::Graphics::cEffect** s_effectArr = nullptr;
	uint16_t s_effectCount = 0;
}


// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

// Render
//-------

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		if (Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread))
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			if (!s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal())
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	// clear view
	if (s_View) {
		s_View->ClearView(0.0f, 0.0f, 0.0f);
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);

#if defined( EAE6320_PLATFORM_D3D )
	auto* const dataRequiredToRenderFrame = s_dataBeingRenderedByRenderThread;
#endif

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
#if defined( EAE6320_PLATFORM_D3D )
		auto& constantData_frame = dataRequiredToRenderFrame->constantData_frame;
#elif defined( EAE6320_PLATFORM_GL )
		auto& constantData_frame = s_dataBeingRenderedByRenderThread->constantData_frame;
#endif
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	// draw mesh
	{
		// first mesh and effect
		{
			// Bind the shading data
			{
				if (s_effectArr[0])
				{
					s_effectArr[0]->BindEffect();
				}
			}
			// Draw the geometry
			{
				if (s_meshArr[0])
				{
					s_meshArr[0]->DrawMesh();
				}
			}
		}

		// second mesh and effect
		{
			// Bind the shading data
			{
				if (s_effectArr[1])
				{
					s_effectArr[1]->BindEffect();
				}
			}
			// Draw the geometry
			{
				if (s_meshArr[1])
				{
					s_meshArr[1]->DrawMesh();
				}
			}
		}
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		if (s_View) {
			s_View->SwapBackBuffer();
		}
	}

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
		//dataRequiredToRenderFrame	// TODO
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without context");
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_frame.Initialize())
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
			return result;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
			return result;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
			return result;
		}
	}
	// Initialize the views
	{
		s_View = new cView();
		if (!(result = s_View->InitializeViews(i_initializationParameters)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the views");
			return result;
		}
	}
	// Initialize the shading data
	{
		// create effect array
		s_effectCount = 2;
		s_effectArr = new cEffect * [s_effectCount];

		// first effect
		{
			cEffect* effect = new cEffect("standard", "myshader");
			if (!(result = effect->InitializeShadingData()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
			s_effectArr[0] = effect;
		}

		// second effect
		{
			cEffect* effect = new cEffect("standard", "standard");
			if (!(result = effect->InitializeShadingData()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
				return result;
			}
			s_effectArr[1] = effect;
		}
	}
	// Initialize the geometry
	{
		// create mesh array
		s_meshCount = 2;
		s_meshArr = new cMesh * [s_meshCount];

		// first mesh
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

			cMesh* mesh = new cMesh(static_cast<unsigned int>(2), static_cast<unsigned int>(3), i_vertexData, i_indices);
			if (!(result = mesh->InitializeGeometry()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
			s_meshArr[0] = mesh;
		}

		// second mesh
		{
			eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData = new eae6320::Graphics::VertexFormats::sVertex_mesh[3];
			{
				// clockwise
				i_vertexData[0].x = 1.5f;
				i_vertexData[0].y = 1.5f;
				i_vertexData[0].z = 0.0f;

				i_vertexData[1].x = 1.8f;
				i_vertexData[1].y = 1.5f;
				i_vertexData[1].z = 0.0f;

				i_vertexData[2].x = 1.8f;
				i_vertexData[2].y = 1.8f;
				i_vertexData[2].z = 0.0f;
			}

			uint16_t* i_indices = new uint16_t[3]{ 0, 1, 2 };

			cMesh* mesh = new cMesh(static_cast<unsigned int>(1), static_cast<unsigned int>(3), i_vertexData, i_indices);
			if (!(result = mesh->InitializeGeometry()))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
				return result;
			}
			s_meshArr[1] = mesh;
		}
	}

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	// view clean up
	{
		if (s_View) {
			result = s_View->CleanUp();
			delete s_View;
		}
	}

	// mesh data clean up
	{
		for (int i = 0; i < s_meshCount; i++) {
			if (s_meshArr[i])
			{
				result = s_meshArr[i]->CleanUp();
				delete s_meshArr[i];
			}
		}
		delete[] s_meshArr;

	}

	// effect data clean up
	{
		for (int i = 0; i < s_effectCount; i++) {
			if (s_effectArr[i])
			{
				result = s_effectArr[i]->CleanUp();
				delete s_effectArr[i];
			}
		}
		delete[] s_effectArr;
	}

	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if (!result_constantBuffer_frame)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_frame;
			}
		}
	}

	{
		const auto result_context = sContext::g_context.CleanUp();
		if (!result_context)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_context;
			}
		}
	}

	return result;
}

