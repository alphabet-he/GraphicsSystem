#include "cEffect.h"
#include <string>

eae6320::Graphics::cEffect::cEffect()
{
	m_vertexShaderPath = "data/Shaders/Vertex/standard.shader";
	m_fragmentShaderPath = "data/Shaders/Fragment/myshader.shader";
}

eae6320::Graphics::cEffect::cEffect(const char* i_vertexShaderFileName, const char* i_fragmentShaderFileName)
{
	m_vertexShaderPath = std::string("data/Shaders/Vertex/") + i_vertexShaderFileName + std::string(".shader");
	m_fragmentShaderPath = std::string("data/Shaders/Fragment/") + i_fragmentShaderFileName + std::string(".shader");

}

eae6320::Graphics::cEffect::~cEffect()
{
}
