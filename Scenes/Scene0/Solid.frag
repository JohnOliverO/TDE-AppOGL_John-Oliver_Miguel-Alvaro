#version 330

out vec4 FragColor;

in vec3 vertexColor;

uniform bool bSolidColor = false;
uniform vec4 vCubeColor = vec4(1.0,1.0,1.0,1.0);

void main()
{
	if(bSolidColor){
		FragColor = vCubeColor;
	}else{
		FragColor = vec4(vertexColor, 1.0);
	}
}