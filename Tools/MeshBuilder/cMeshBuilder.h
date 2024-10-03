/*
	This class builds meshes
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/cMesh.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;

			// Implementation
			//===============

		private:

			// Build
			//------

			cResult Build(const Graphics::eShaderType i_shaderType, const std::vector<std::string>& i_arguments);
		};
	}
}

#endif	// EAE6320_CSHADERBUILDER_H
