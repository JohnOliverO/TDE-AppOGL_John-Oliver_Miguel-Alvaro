#version 330

out vec4 FragColor;

in vec3 Normal;

in vec2 TexCoord0;
in vec2 TexCoord1;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform vec4 Color = vec4(1.0,1.0,1.0,1.0);

void main()
{
	vec4 tex0 = texture(Texture0, TexCoord0);
	vec4 tex1 = texture(Texture1, TexCoord1);

	FragColor = (tex0 * tex1);
}