#version 330

layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float fTime;
uniform vec2 vScrollDirection = vec2(1.0, 0.0);

uniform vec2 Tile = vec2(5.0, 5.0);

out vec3 Normal;
out vec2 TexCoord0;
out vec2 TexCoord1;

void main()
{
	Normal = aNormal;

	//TexCoord0 = vec2(aTexCoord.s * Tile.s, aTexCoord.t * Tile.t);
	TexCoord0 = aTexCoord + vec2(1.0, 0.0) * (fTime * 0.02f);
	TexCoord1 = aTexCoord + vec2(1.0, 0.3) * (fTime * 0.01f);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}