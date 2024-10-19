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
	float red = 0.5 + 0.5 * sin( g_elapsedSecondCount_simulationTime );
	float green = 0.5 + 0.5 * cos( g_elapsedSecondCount_simulationTime );

	PLATFORM_float4 calculated_color = PLATFORM_float4(
		// RGB (color)
		red, green, 0.5,
		// Alpha (opacity)
		1.0 );

	o_color = calculated_color * i_fragmentColor;
	}