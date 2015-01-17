#version 130

uniform sampler2D texture0;
uniform vec4 color;

in vec2 texCoord0;
out vec4 fragColor;

void main(void)
{
	vec4 textureColor = texture2D( texture0, texCoord0 );
	
	if( textureColor.rgb != vec3( 1.0, 1.0, 1.0 ) )
		fragColor = vec4( textureColor.rgb, textureColor.r ) * color;

	else
		fragColor = textureColor * color;
}

