/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define PLATFORM_float4 float4
    #define PLATFORM_float4x4 float4x4
    #define PLATFORM_float2 float2

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define PLATFORM_float4 vec4
    #define PLATFORM_float4x4 mat4
    #define PLATFORM_float2 vec2

#endif

DeclareConstantBuffer( g_constantBuffer_frame, 0 )
{
    PLATFORM_float4x4 g_transform_worldToCamera;
    PLATFORM_float4x4 g_transform_cameraToProjected;

    float g_elapsedSecondCount_systemTime;
    float g_elapsedSecondCount_simulationTime;
    // For alignment
    PLATFORM_float2 g_padding;
};

// Fragment
//=================

#if defined( EAE6320_PLATFORM_D3D )
    # define PLATFORM_OutputFragmentShader(i_name) void i_name(in const float4 i_fragmentPosition : SV_POSITION, out float4 o_color : SV_TARGET)

#elif defined( EAE6320_PLATFORM_GL )
    # define PLATFORM_OutputFragmentShader(i_name) void i_name()
#endif


// Vertex
//=================

#if defined( EAE6320_PLATFORM_D3D )
    # define PLATFORM_Mul(matrix, vector) mul( matrix, vector )
    # define PLATFORM_OutputVertexShader(i_name) void i_name(in const float3 i_vertexPosition_local : POSITION, out float4 o_vertexPosition_projected : SV_POSITION)

#elif defined( EAE6320_PLATFORM_GL )
    # define PLATFORM_Mul(matrix, vector) matrix * vector
    # define PLATFORM_OutputVertexShader(i_name) void i_name()
#endif