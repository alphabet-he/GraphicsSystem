/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>


PLATFORM_OutputFragmentShader{
	float red = 0.5 + 0.5 * sin( g_elapsedSecondCount_simulationTime );
	float green = 0.5 + 0.5 * cos( g_elapsedSecondCount_simulationTime );
	// Output solid white
	o_color = PLATFORM_float4(
		// RGB (color)
		red, green, 0.5,
		// Alpha (opacity)
		1.0 );
	}