#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"
#include "Engine/CPrimitives.h"



class Scene3 : public CSceneBase
{
	public:
		Scene3();
		~Scene3();

		virtual void DrawScene(float deltaTime);
		virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		void DrawHouse(glm::vec3 position = glm::vec3(0.0f), float rotationAngle = 0.0f, glm::vec3 scale = glm::vec3(1.0f));

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

		CPrimitives pPlane, pBox;

		std::vector<CPrimitives> pHouse;

};

