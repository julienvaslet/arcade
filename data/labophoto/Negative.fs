#version 130

uniform sampler2D texture0;

in vec2 texCoord0;
out vec4 outColor;

void main(void)
{
	vec4 color = texture( texture0, texCoord0 );
	outColor = vec4( 1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a );
}

