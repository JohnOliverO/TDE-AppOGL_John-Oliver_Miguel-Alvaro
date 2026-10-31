#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D Texture0;
uniform float AmbientStrength = 0.1;
uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform float Shininess = 32;

void main()
{
    vec3 light = vec3(1.0, 1.0, 1.0);

    // AMBIENT LIGHT //
    // Calcula o componente de luz ambiente
    vec3 ambient = AmbientStrength * LightColor;

    // DIFFUSE LIGHT //
    // Normaliza o vector Normal
    vec3 norm = normalize(Normal);

    // Vetor de direção do raio de luz
    vec3 LightDir = normalize(LightPos - FragPos);

    // Fator de iluminação difusa, Angulo entre os vetores
    float diff = max(dot(norm, LightDir), 0.0);
    // Calcula o componente de luz difusa
    vec3 diffuse = diff * LightColor;

    // SPECULAR LIGHT //
    // Calcula o vetor do fragmento até a posição da câmera
    vec3 viewDir = normalize(ViewPos - FragPos);

    // Calcula o vetor de reflexão da luz
    vec3 reflectDir = reflect(-LightDir, norm);

    // calcula o angulo do vetor de reflexão
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);

    // Calcula o componente de luz especular
    vec3 specular = spec * LightColor;


    // Valor final de todos os componentees da luz
    light = ambient + diffuse + specular;

    // Cor do texel da textura
    vec4 texColor = texture(Texture0, TexCoord);

    // Descarta os pixel do canal Alpha da textura .PNG
    texColor.rgb *= texColor.a;
    if(texColor.a < 0.1) // Adjust threshold as needed
        discard;

    // Cor final do fragmento
    FragColor = vec4(light, 1.0) * texColor;
}