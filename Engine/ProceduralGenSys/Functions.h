#ifndef EAE6320_PGS_FUNCTIONS_H
#define EAE6320_PGS_FUNCTIONS_H

#include "Engine/Graphics/cMesh.h"

namespace eae6320
{
	namespace Assets {
		struct sProceduralGeneratedMesh
		{
			uint16_t m_staticMeshCount = 0;
			Graphics::cMesh** m_staticMeshArr;
		};

		static cResult Load(const char* i_meshDataDirName, sProceduralGeneratedMesh*& o_generatedMesh);
	}
}

#endif
