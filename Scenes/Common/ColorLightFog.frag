#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 Vertex;

out vec4 FragColor;

uniform sampler2D Texture0;

uniform float ambientStrength = 0.1;
uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform float Shininess = 32.0;

uniform vec3 CameraPos;
uniform vec4 FogColor = vec4(0.5, 0.5, 0.5, 1.0);
uniform float FogDensity = 0.03;
uniform int FogMode = 0; // 0: linear, 1: exponential, 2: exponential squared
uniform float FogStart = 0.1;
uniform float FogEnd = 100.0;
uniform bool FogEnabled = true;


float CalculateFogFactor(float C) // C is the distance from the camera to the fragment
{
    if (C >= FogEnd) return 1.0;
    if (C <= FogStart) return 0.0;

    float fog = 0.0;
    float d = FogDensity;

    switch (FogMode)
    {
        case 0: // Linear
            fog = 1.0 - (FogEnd - C) / (FogEnd - FogStart);
            break;
        
        case 1: // Exponential
            fog = 1.0 - exp(-d * C);
            break;
        
        case 2: // Exponential Squared
            fog = 1.0 - exp(-d * d * C * C);
            break;
    }
    return fog;
}

float CalculateFogFactor()
{
    // Calculate fog factor based on distance from camera
    float C = distance(CameraPos, Vertex);

    // Apply fog effect if enabled
    float fogFactor = CalculateFogFactor(C);

    return fogFactor;
}


vec3 CalculateLight()
{
    vec3 light = vec3(1.0, 1.0, 1.0);

    // AMBIENT LIGHT //
    // Calcula o componente de luz ambiente
    vec3 ambient = ambientStrength * LightColor;

    // DIFFUSE LIGHT //
    // Normaliza o vetor Normal
    vec3 norm = normalize(Normal);
    // Vetor de direção do raio de luz
    vec3 lightDir = normalize(LightPos - FragPos);
    // Fator de iluminação difusa. Angulo entre os vetores
    float diff = max(dot(norm, lightDir), 0.0);
    // Calcula o componente de luz difusa
    vec3 diffuse = diff * LightColor;

    
    // SPECULAR LIGHT //
    // Calcula o vetor do fragmento até a posição da câmera
    vec3 viewDir = normalize(ViewPos - FragPos);
    // Calcula o vetor de reflexão da luz
    vec3 reflectDir = reflect(-lightDir, norm);
    // Calcula o angulo do vetor de reflexão
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess); 
    // Calcula o componente de luz especular
    vec3 specular = spec * LightColor; 


    // Valor final de todos os componentes da luz
    light = ambient + diffuse + specular;
    
    return light;
}


vec4 CalculateTextureColor()
{
    // Cor do texel da textura
    vec4 texColor = texture(Texture0, TexCoord);

    // Descarta os pixel do canal Alpha da textura .PNG
    texColor.rgb *= texColor.a;
    if(texColor.a < 0.1) // Adjust threshold as needed
        discard;

    return texColor;
}


void main()
{
    vec4 textureColor = CalculateTextureColor();
    vec3 lightColor = CalculateLight();
    float fogFactor = CalculateFogFactor();
    
    // Combina a cor da textura com a cor da luz
    vec3 finalColor = textureColor.rgb * lightColor;

    // Mistura a cor final com a cor do nevoeiro com base no fator de nevoeiro
    vec3 foggedColor = mix(finalColor, FogColor.rgb, fogFactor);
    
    // Cor final do fragmento
    FragColor = vec4(foggedColor, textureColor.a);
}