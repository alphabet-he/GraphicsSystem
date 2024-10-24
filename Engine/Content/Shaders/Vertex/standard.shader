/*
	This is the standard vertex shader

	A vertex shader is responsible for two things:
		* Telling the GPU where the vertex (one of the three in a triangle) should be drawn on screen in a given window
			* The GPU will use this to decide which fragments (i.e. pixels) need to be shaded for a given triangle
		* Providing any data that a corresponding fragment shader will need
			* This data will be interpolated across the triangle and thus vary for each fragment of a triangle that gets shaded
*/

#include <Shaders/shaders.inc>

#if defined(EAE6320_PLATFORM_GL)
	layout( location = 0 ) in vec3 i_vertexPosition_local;

	layout( location = 1 ) in vec4 i_vertexColor;
	layout( location = 0 ) out vec4 o_vertexColor;
#endif

PLATFORM_OutputVertexShader(main)
{
	// Transform the local vertex into world space
	PLATFORM_float4 vertexPosition_world;
	{
		// This will be done in a future assignment.
		// For now, however, local space is treated as if it is the same as world space.
		PLATFORM_float4 vertexPosition_local = PLATFORM_float4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = PLATFORM_Mul(g_transform_localToWorld, vertexPosition_local);
		//vertexPosition_world = vertexPosition_local;
	}
	// Calculate the position of this vertex projected onto the display
	{
		// Transform the vertex from world space into camera space
		PLATFORM_float4 vertexPosition_camera = PLATFORM_Mul( g_transform_worldToCamera, vertexPosition_world );
		// Project the vertex from camera space into projected space

		#if defined(EAE6320_PLATFORM_D3D)
			o_vertexPosition_projected = PLATFORM_Mul( g_transform_cameraToProjected, vertexPosition_camera );
		#elif defined(EAE6320_PLATFORM_GL)
			gl_Position = PLATFORM_Mul( g_transform_cameraToProjected, vertexPosition_camera );
		#endif
			
	}

	// color
	o_vertexColor = i_vertexColor;
}
