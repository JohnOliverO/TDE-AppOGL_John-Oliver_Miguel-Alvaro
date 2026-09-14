#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"
#include "Engine/CPrimitives.h"



class Scene1 : public CSceneBase
{
public:
		Scene1();
		~Scene1();

		virtual void DrawScene(float deltaTime);
        virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		void DrawTree(glm::vec3 pos, glm::vec3 size = glm::vec3(1.0, 1.0, 1.0));

		// forma adicional para entrega de trabalho
		// circle
		void CreateCircle(float radius, int segments);
		void DrawCircle();
		void DestroyCircle();

private:

	// Camera
	CCamera* pCamera;

	// Axis
	CGridAxis* pGridAxis;

	// Shader
	CShader* pShader;

	// Textures
	CTextures* pTextures;

	// Timer
	CTimer* pTimer;			

	// Texto
	CText* pText;			

	// Primitivas
	CPrimitives cube, sphere, cylinder, cone;

	// forma adicional para entrega de trabalho
	GLuint circleVAO, circleVBO;
	// numero de segmentos do círculo
	int numSegments;
};

