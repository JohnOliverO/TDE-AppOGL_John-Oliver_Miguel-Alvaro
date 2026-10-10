#version 330

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D Texture0;
uniform vec4 Color = vec4(1.0,1.0,1.0,1.0);

void main()
{
	//FragColor = texture(Texture0, TexCoord);

	FragColor = Color;
}