#version 100
precision mediump float;

uniform sampler2D texture0;
uniform vec4 color;

varying vec2 texCoord;

void main(void)
{
	vec4 textureColor = texture2D( texture0, texCoord );
	
	if( textureColor.rgb != vec3( 1.0, 1.0, 1.0 ) )
		gl_FragColor = vec4( textureColor.rgb, textureColor.r ) * color;

	else
		gl_FragColor = textureColor * color;
}

