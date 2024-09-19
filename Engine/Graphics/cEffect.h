#pragma once
#ifndef EAE6320_GRAPHICS_CEFFECT_H
#define EAE6320_GRAPHICS_CEFFECT_H

#include "cShader.h"
#include "cRenderState.h"

namespace eae6320 
{
	namespace Graphics 
	{
		class cEffect 
		{
			// variables
			eae6320::Graphics::cShader* m_vertexShader;
			eae6320::Graphics::cShader* m_fragmentShader;

			eae6320::Graphics::cRenderState m_renderState;

			const char* m_vertexShaderFileName;
			const char* m_fragmentShaderFileName;

#if defined( EAE6320_PLATFORM_GL )
			GLuint m_programId = 0;
#endif

		public:
			void BindEffect();

			eae6320::cResult InitializeShadingData();

			eae6320::cResult CleanUp();

			cEffect();

			cEffect(const char* i_vertexShaderFileName, const char* i_fragmentShaderFileName);

			~cEffect();
		};
	}
}

#endif
