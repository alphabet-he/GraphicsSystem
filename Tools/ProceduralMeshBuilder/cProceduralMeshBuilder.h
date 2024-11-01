/*
	This class builds meshes
*/

#ifndef EAE6320_CPROCEDURALMESHBUILDER_H
#define EAE6320_CPROCEDURALMESHBUILDER_H

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
		class cProceduralMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;
		};
	}
}

#endif	// EAE6320_CSHADERBUILDER_H
