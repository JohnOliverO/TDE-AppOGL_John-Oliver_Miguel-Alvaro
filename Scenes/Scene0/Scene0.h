#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"



class Scene0 : public CSceneBase
{
public:
	Scene0();
	~Scene0();

	virtual void DrawScene(float deltaTime);
	virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

	void ShowGUI();
	//triangle
	void CreateTriangle();
	void DrawTriangle();
	void DestroyTriangle();
	//square
	void CreateSquare();
	void DrawSquare();
	void DestroySquare();
	//cube
	void CreateCube();
	void DrawCube();
	void DestroyCube();
	//box
	void CreateBox(float xSize, float ySize, float zSize);
	void DrawBox();
	void DestroyBox();
	//triangular-based pyramid
	void CreateTriangularPyramid(float baseSize, float height);
	void DrawTriangularPyramid();
	void DestroyTriangularPyramid();
	//square-based pyramid
	void CreateSquarePyramid(float baseSize, float height);
	void DrawSquarePyramid();
	void DestroySquarePyramid();
	/*
	//pentagonal-based pyramid
	void CreatePentagonalPyramid(float baseSize, float height);
	void DrawPentagonalPyramid();
	void DestroyPentagonalPyramid();
	//hexagonal-based pyramid
	void CreateHexagonalPyramid(float baseSize, float height);
	void DrawHexagonalPyramid();
	void DestroyHexagonalPyramid();
	//heptagonal-based pyramid
	void CreateHeptagonalPyramid(float baseSize, float height);
	void DrawHeptagonalPyramid();
	void DestroyHeptagonalPyramid();
	//octagonal-based pyramid
	void CreateOctagonalPyramid(float baseSize, float height);
	void DrawOctagonalPyramid();
	void DestroyOctagonalPyramid();
	*/

	//Plane
	void CreatePlane();
	void DrawPlane();
	void DestroyPlane();

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

	// Cor do Framebuffer
	glm::vec4 vFramebufferColor;

	bool bActiveGUI;

	//Declara o Vertex Array Object e o Vertex Buffer Object
	GLuint triangleVAO, triangleVBO;
	GLuint squareVAO, squareVBO;
	GLuint cubeVAO, cubeVBO;
	GLuint boxVAO, boxVBO;
	GLuint triangularPyramidVAO, triangularPyramidVBO;
	GLuint squarePyramidVAO, squarePyramidVBO;
	GLuint planeVAO, planeVBO;

	// Variáveis que controlam a translação do cubo atraves das setas do teclado.
	float fPosX, fPosY, fPosZ, fSpeed;

	//Vetor que controla a escala do Cubo
	glm::vec3 vScale;

	//Controla a velocidade de rotação da pirâmide
	float fRotSpeed;

	//Habilita ou Desabilita a rotação da pirâmide
	bool bRot;

	//Cor sólida para o cubo
	glm::vec4 vCubeColor;

	//Habilita ou Desabilita cor sólida
	bool bCubeColor;

	//
	glm::vec2 Tile;
};

