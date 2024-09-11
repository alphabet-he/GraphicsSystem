#pragma once

#ifndef EAE6320_GRAPHICS_CVIEW_H
#define EAE6320_GRAPHICS_CVIEW_H

#include <Engine/Results/cResult.h>
#include <Engine/Graphics/sContext.h>
#include <Engine/Asserts/Asserts.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

#if defined( EAE6320_PLATFORM_D3D )
	#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
	#include "OpenGL/Includes.h"
#endif

namespace eae6320 
{
	namespace Graphics 
	{
		class cView 
		{
#if defined( EAE6320_PLATFORM_D3D )

			// In Direct3D "views" are objects that allow a texture to be used a particular way:
			// A render target view allows a texture to have color rendered to it
			ID3D11RenderTargetView* s_renderTargetView = nullptr;

			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* s_depthStencilView = nullptr;
#endif
		public:
			eae6320::cResult InitializeViews(const sInitializationParameters& i_initializationParameters);

			eae6320::cResult CleanUp();

			void ClearView();
			
			void SwapBackBuffer();

			cView();
			~cView();

		};
	}

}

#endif