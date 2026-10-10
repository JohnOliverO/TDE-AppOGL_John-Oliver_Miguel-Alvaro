#version 330

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform vec4 Color = vec4(1.0,1.0,1.0,1.0);

void main()
{
	FragColor = Color;
}