#version 100
precision mediump float;

uniform sampler2D texture;

varying vec2 texCoord;
varying vec4 color;

void main(void)
{
	gl_FragColor = texture2D( texture, texCoord ) * color;
}

