#version 130

uniform sampler2D texture;
uniform vec4 color;

in vec2 texCoord;
out vec4 outColor;

void main(void)
{
	vec4 textureColor = texture2D( texture, texCoord );
	
	if( textureColor.rgb != vec3( 1.0f, 1.0f, 1.0f ) )
		outColor = vec4( textureColor.rgb, textureColor.r ) * color;

	else
		outColor = textureColor * color;
}

