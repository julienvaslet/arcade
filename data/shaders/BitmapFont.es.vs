#version 100
precision mediump float;

uniform vec2 window;

attribute vec2 a_Vertex;
attribute vec2 a_TexCoord;
varying vec2 texCoord;

void main( void )
{
	texCoord = a_TexCoord;
	gl_Position = vec4( (a_Vertex.x * 2.0 / window.x) - 1.0, (a_Vertex.y * 2.0 / window.y) - 1.0, 0.5, 1.0 );
}
