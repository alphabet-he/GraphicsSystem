#pragma once

#ifndef EAE6320_GRAPHICS_CMESH_H
#define EAE6320_GRAPHICS_CMESH_H

#include <Engine/Results/cResult.h>
#include "VertexFormats.h"
#include "cVertexFormat.h"

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
		class cMesh 
		{
			// variables

			unsigned int m_triangleCount;
			unsigned int m_vertexCountPerTriangle;
			eae6320::Graphics::VertexFormats::sVertex_mesh* m_vertexData = nullptr;
			uint16_t* m_indices = nullptr;

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh);

			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::cVertexFormat* m_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;

			// An index buffer
			ID3D11Buffer* m_indexBuffer = nullptr;

#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;

			// A vertex buffer holds the data for each vertex
			GLuint m_indexBufferId = 0;
#endif


			// functions
		public:

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

			static cResult Load(unsigned int i_triangleCount,
				unsigned int i_vertexCountPerTriangle,
				eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[],
				uint16_t i_indices[], 
				cMesh*& o_mesh);

			void DrawMesh();

		private:

			eae6320::cResult InitializeGeometry();

			eae6320::cResult CleanUp();

			cMesh();
			cMesh(unsigned int i_triangleCount,
				unsigned int i_vertexCountPerTriangle,
				eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[],
				uint16_t i_indices[]); // use OpenGL right handed winding order (counter clockwise)
			~cMesh();
		};
	}
}

#endif