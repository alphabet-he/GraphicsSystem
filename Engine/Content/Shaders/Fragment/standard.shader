/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined(EAE6320_PLATFORM_GL)
	layout( location = 0 ) in PLATFORM_float4 i_fragmentColor;
	out PLATFORM_float4 o_color;
#endif

PLATFORM_OutputFragmentShader(main){
	o_color = i_fragmentColor;
	}
