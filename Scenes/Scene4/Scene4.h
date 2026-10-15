#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"
#include "Engine/CPrimitives.h"



class Scene4 : public CSceneBase
{
	public:
		Scene4();
		~Scene4();

		virtual void DrawScene(float deltaTime);
		virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		int HouseLoader(std::string filename);
		void DrawHouse(int vertexCount, glm::vec3 position = glm::vec3(0.0f), float rotationAngle = 0.0f, glm::vec3 scale = glm::vec3(1.0f));
		void DestroyHouse();

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

		GLuint HouseVAO, HouseVBO;

		int HouseVertexCount;

		CPrimitives pPlane;
};

