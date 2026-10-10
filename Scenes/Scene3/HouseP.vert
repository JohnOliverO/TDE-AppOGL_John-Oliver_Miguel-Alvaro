#version 330

layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int TextureType = 0;

out vec3 Normal;
out vec2 TexCoord;

void main()
{
	Normal = aNormal;

	// Reduz a escala da textura original para metade do tamanho (0.0 a 0.5)
	vec2 scaledTex = aTexCoord * 0.5;

	// Determina o deslocamento (offset) baseado no ID da textura
	vec2 offset = vec2(0.0, 0.0);

	// Mapeamento dos 4 quadrantes (0, 1, 2, 3) de um Texture Atlas 2x2
	if (TextureType == 0) offset = vec2(0.0, 0.5); // Quadrante Superior Esquerdo
	if (TextureType == 1) offset = vec2(0.5, 0.5); // Quadrante Superior Direito
	if (TextureType == 2) offset = vec2(0.0, 0.0); // Quadrante Inferior Esquerdo
	if (TextureType == 3) offset = vec2(0.5, 0.0); // Quadrante Inferior Direito

	// Aplica o corte do quadrante
	TexCoord = scaledTex + offset;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}