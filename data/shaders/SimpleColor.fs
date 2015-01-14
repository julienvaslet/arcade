#ifdef GL_ES
#version 300 es
precision mediump float;
#else
#version 130
#endif

in vec4 color;
out vec4 outColor;

void main(void)
{
	outColor = color;
}

