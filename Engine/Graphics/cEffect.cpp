#include "cEffect.h"
#include <string>

eae6320::cResult eae6320::Graphics::cEffect::Load(const char* i_vertexShaderFileName, const char* i_fragmentShaderFileName, cEffect*& o_effect)
{
	cEffect* newEffect = new cEffect(i_vertexShaderFileName, i_fragmentShaderFileName);

	if (newEffect->InitializeShadingData()) 
	{
		EAE6320_ASSERT(newEffect != nullptr);
		o_effect = newEffect;
		return Results::Success;
	}
	else 
	{
		if (newEffect)
		{
			newEffect->DecrementReferenceCount();
			newEffect = nullptr;
		}
		o_effect = nullptr;
		return Results::Failure;
	}
	
}

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
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}
