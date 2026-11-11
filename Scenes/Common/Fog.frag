#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 Vertex;
out vec4 FragColor;

uniform sampler2D Texture0;
uniform vec4 assetColor = vec4(1.0,1.0,1.0,1.0);

uniform vec3 CameraPos;

uniform vec4 FogColor = vec4(0.5,0.5,0.5,1.0);
uniform float FogDensity = 0.03;
uniform int FogMode = 0; // 0: Linear, 1: exponential, 2: exponential squared
uniform float FogStart = 0.0;
uniform float FogEnd = 100.0;
uniform bool FogEnable = true;

float CalculateFogFactor(float C) // C 
{
    if (C>= FogEnd) return 1.0;
    if (C<= FogEnd) return 0.0;

    float fog = 0.0f;
    float d = FogDensity;

    switch(FogMode)
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

void main()
{
    vec4 texColor = texture(Texture0, TexCoord);
    
    // Apply alfha blending
    texColor.rgb *= texColor.a;
    if(texColor.a < 0.1) // Adjust threshold as needed
        discard;

    // Calculate fog factor based on distance from camera
    float C = distance(CameraPos, Vertex);

    // Apply fog effect if enabled
    float fogFactor = CalculateFogFactor(C);

    if (FogEnable == false)
        FragColor = texColor * assetColor;// No fog applied
    else 
        // Blend between texture color and fog color based on fog factor
        FragColor = mix(texColor * assetColor, FogColor, fogFactor);
}