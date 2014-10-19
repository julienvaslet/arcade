#version 130

uniform sampler2D texture;

in vec2 texCoord;
out vec4 outColor;

void main(void)
{
	outColor = texture2D( texture, texCoord );
}

