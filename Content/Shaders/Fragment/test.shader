/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Constant Buffers
//=================

cbuffer g_constantBuffer_frame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

cbuffer g_constantBuffer_drawCall : register( b2 )
{
  float4x4 g_transform_localToWorld;
};

// Entry Point
//============

void main(

	// Input
	//======

	in const float4 i_fragmentPosition : SV_POSITION,
	in const float4 i_vertex_color : COLOR,
	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

)
{
	// Output input color
	o_color = i_vertex_color;
		
		float4 animated_color = float4(1.0, 1.0, 0.0, 1.0);
		animated_color.r = sin(g_elapsedSecondCount_simulationTime);
		animated_color.g = cos(g_elapsedSecondCount_simulationTime);
		
		//o_color = o_color * animated_color;
}

#elif defined( EAE6320_PLATFORM_GL )

// Constant Buffers
//=================

layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For vec4 alignment
	vec2 g_padding;
};

layout( std140, binding = 2 ) uniform g_constantBuffer_drawCall
{
  mat4 g_transform_localToWorld;
};

// Input
//======
layout( location = 1 ) in vec4 i_vertex_color;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()
{
	// calculate and Output
	
		vec4 animated_color = vec4(1.0, 1.0, 0.0, 1.0);;
		animated_color.r = sin(g_elapsedSecondCount_simulationTime);
		animated_color.g = cos(g_elapsedSecondCount_simulationTime);
		
		o_color = i_vertex_color * animated_color;
}

#endif
