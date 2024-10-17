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
	cEffect("standard", "myshader");
}

eae6320::Graphics::cEffect::cEffect(const char* i_vertexShaderFileName, const char* i_fragmentShaderFileName)
{

	std::string i_vertexPath = std::string("data/Shaders/Vertex/") + i_vertexShaderFileName + std::string(".shader");
	std::string i_fragmentPath = std::string("data/Shaders/Fragment/") + i_fragmentShaderFileName + std::string(".shader");

	auto result = eae6320::Results::Success;

	if (!(result = eae6320::Graphics::cShader::Load(i_vertexPath,
		m_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return;
	}
	if (!(result = eae6320::Graphics::cShader::Load(i_fragmentPath,
		m_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return;
	}
	{
		constexpr auto renderStateBits = []
			{
				uint8_t renderStateBits = 0;

				eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
				eae6320::Graphics::RenderStates::EnableDepthTesting(renderStateBits);
				eae6320::Graphics::RenderStates::EnableDepthWriting(renderStateBits);
				eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

				return renderStateBits;
			}();
		if (!(result = m_renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return;
		}
	}
}

eae6320::Graphics::cEffect::~cEffect()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}
