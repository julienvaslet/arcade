#version 130

uniform sampler2D texture;

in vec2 texCoord;
in vec4 color;
out vec4 outColor;

void main(void)
{
	outColor = texture2D( texture, texCoord ) * color;
}

