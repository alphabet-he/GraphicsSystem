#include "cMeshBuilder.h"
#include "Engine/Platform/Platform.h"
#include <Tools/AssetBuildLibrary/Functions.h>

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
    cResult result = eae6320::Platform::CopyFile(m_path_source, m_path_target);
    if (!result) {
        OutputErrorMessageWithFileInfo(m_path_source, "Copy mesh file failed.");
    }
    return cResult();
}
