#version 330

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D Texture0;//Grass
uniform sampler2D Texture1;//Mask
uniform sampler2D Texture2;//Mud

void main()
{
	vec4 Tex0 = texture(Texture0, TexCoord);
	vec4 Tex1 = texture(Texture1, TexCoord);
	vec4 Tex2 = texture(Texture2, TexCoord);

	FragColor = Tex0 * Tex1 + Tex2 * (1.0 - Tex1);
}