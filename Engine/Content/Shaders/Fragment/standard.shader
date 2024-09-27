/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined(EAE6320_PLATFORM_GL)
	out PLATFORM_float4 o_color;
#endif

PLATFORM_OutputFragmentShader(main){
	// Output solid white
	o_color = PLATFORM_float4(
		// RGB (color)
		1.0, 1.0, 1.0,
		// Alpha (opacity)
		1.0 );
	}
