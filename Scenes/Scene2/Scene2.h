#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"
#include "Engine/CPrimitives.h"
#include <stack>

struct StackInfo
{
	CPrimitives Shape;
	glm::vec3 positions = glm::vec3(-2.0f, 0.0f, 0.0f);
	std::stack<glm::vec4> cubeColors;
	int stackHeight = 10;
};


class Scene2 : public CSceneBase
{
	public:
		Scene2();
		~Scene2();

		virtual void DrawScene(float deltaTime);
		virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		void DrawStackOfCubes(StackInfo& stackInfo);


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

		// Controle de tempo para a criação de cubos
		float cubeSpawnTimer = 0.0f;

		StackInfo stackInfo0, stackInfo1, stackInfo2, stackInfo3, stackInfo4;

		std::vector<bool> fullStacks = {false, false, false, false, false};
};