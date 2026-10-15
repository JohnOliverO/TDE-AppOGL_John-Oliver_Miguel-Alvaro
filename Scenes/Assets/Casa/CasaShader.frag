#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture0;

void main()
{
	FragColor = texture(Texture0, TexCoord);
}