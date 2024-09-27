/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined(EAE6320_PLATFORM_GL)
	out PLATFORM_float4 o_color;
#endif

PLATFORM_OutputFragmentShader(main){
	float red = 0.5 + 0.5 * sin( g_elapsedSecondCount_simulationTime );
	float green = 0.5 + 0.5 * cos( g_elapsedSecondCount_simulationTime );
	// Output solid white
	o_color = PLATFORM_float4(
		// RGB (color)
		red, green, 0.5,
		// Alpha (opacity)
		1.0 );
	}