#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"



class Scene2 : public CSceneBase
{
	public:
		Scene2();
		~Scene2();

		virtual void DrawScene(float deltaTime);
		virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);


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
};

