#include "cEffect.h"
#include <string>

eae6320::Graphics::cEffect::cEffect()
{
	m_vertexShaderFileName = "standard";
	m_fragmentShaderFileName = "myshader";
}

eae6320::Graphics::cEffect::cEffect(const char* i_vertexShaderFileName, const char* i_fragmentShaderFileName)
{
	m_vertexShaderFileName = i_vertexShaderFileName;
	m_fragmentShaderFileName = i_fragmentShaderFileName;
}

eae6320::Graphics::cEffect::~cEffect()
{
}
