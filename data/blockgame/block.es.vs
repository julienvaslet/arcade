#version 100
precision mediump float;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec3 a_Vertex;
attribute vec2 a_TexCoord;
attribute vec4 a_Color;
varying vec2 texCoord;
varying vec4 color;

void main( void )
{
	texCoord = a_TexCoord;
	color = a_Color;
	gl_Position = projection_matrix * modelview_matrix * vec4( a_Vertex, 1.0 );
}

