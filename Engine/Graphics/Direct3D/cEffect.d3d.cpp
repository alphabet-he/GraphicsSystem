#include "../cEffect.h"
#include "Includes.h"
#include "../sContext.h"
#include <Engine/Logging/Logging.h>

void eae6320::Graphics::cEffect::BindEffect()
{
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	{
		constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
		constexpr unsigned int interfaceCount = 0;
		// Vertex shader
		{
			EAE6320_ASSERT((m_vertexShader != nullptr) && (m_vertexShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->VSSetShader(m_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
		}
		// Fragment shader
		{
			EAE6320_ASSERT((m_fragmentShader != nullptr) && (m_fragmentShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->PSSetShader(m_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
		}
	}
	// Render state
	{
		m_renderState.Bind();
	}
}

eae6320::cResult eae6320::Graphics::cEffect::InitializeShadingData()
{
	auto result = eae6320::Results::Success;

	std::string i_vertexPath = std::string("data/Shaders/Vertex/") + m_vertexShaderFileName + std::string(".shader");
	std::string i_fragmentPath = std::string("data/Shaders/Fragment/") + m_fragmentShaderFileName + std::string(".shader");

	if (!(result = eae6320::Graphics::cShader::Load(i_vertexPath,
		m_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(i_fragmentPath,
		m_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
			{
				uint8_t renderStateBits = 0;

				eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
				eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
				eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
				eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

				return renderStateBits;
			}();
		if (!(result = m_renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

	return result;
}

eae6320::cResult eae6320::Graphics::cEffect::CleanUp()
{
	auto result = Results::Success;

	if (m_vertexShader)
	{
		m_vertexShader->DecrementReferenceCount();
		m_vertexShader = nullptr;
	}
	if (m_fragmentShader)
	{
		m_fragmentShader->DecrementReferenceCount();
		m_fragmentShader = nullptr;
	}

	return result;
}