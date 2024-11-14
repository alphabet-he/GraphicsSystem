#include "sProceduralGeneratedMesh.h"
#include "Engine/Platform/Platform.h"

eae6320::cResult eae6320::Assets::sProceduralGeneratedMesh::Load(const char* i_meshDataDirName, sProceduralGeneratedMesh*& o_generatedMesh)
{
	auto result = eae6320::Results::Success;
	std::vector<std::string> i_pgsFiles;

	std::string i_dirPath = std::string("data/pgs/");
	if (i_meshDataDirName && !std::string(i_meshDataDirName).empty())
	{
		i_dirPath = std::string("data/pgs/") + i_meshDataDirName + "/";
	}

	result = Platform::GetFilesInDirectory(i_dirPath, i_pgsFiles);
	if (!result) {
		EAE6320_ASSERTF(false, "Can't load pgs files");
		return result;
	}

	o_generatedMesh->m_staticMeshCount = static_cast<uint16_t>(i_pgsFiles.size());
	o_generatedMesh->m_staticMeshArr = new Graphics::cMesh * [o_generatedMesh->m_staticMeshCount];

	for (uint16_t i = 0; i < i_pgsFiles.size(); i++) {
		Graphics::cMesh* i_Mesh;
		result = Graphics::cMesh::Load(i_pgsFiles[i].c_str(), i_Mesh);
		if (!result) {
			std::string errorMessage = "Cannot load data from " + i_pgsFiles[i];
			EAE6320_ASSERTF(false, errorMessage.c_str());
			return result;
		}
		o_generatedMesh->m_staticMeshArr[i] = i_Mesh;
	}
	return result;
}
