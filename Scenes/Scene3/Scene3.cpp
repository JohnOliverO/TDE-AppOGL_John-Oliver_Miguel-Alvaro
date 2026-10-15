#include "Scene3.h"


Scene3::Scene3()
{
	// Cria a câmera
	pCamera = NULL;
	pCamera = new CCamera(glm::vec3(0.0f, 3.0f, 10.0f));
	pCamera->MovementSpeed = 100.0f;

	// Cria o Timer
	pTimer = NULL;
	pTimer = new CTimer();
	pTimer->Init();

	// Cria gerenciador de impressão de texto na tela
	pText = NULL;
	pText = new CText();
	pText->InitTextManager();

	// Cria os shaders
	pShader = NULL;
	pShader = new CShader();
	pShader->LoadShader("Grid", "Scenes/Common/grid.vert", "Scenes/Common/grid.frag");
	pShader->LoadShader("Axis", "Scenes/Common/axis.vert", "Scenes/Common/axis.frag");
	pShader->LoadShader("Text2D", "Scenes/Common/Text2D.vert", "Scenes/Common/Text2D.frag");

	pShader->LoadShader("BaseP", "Scenes/Scene3/BaseP.vert", "Scenes/Scene3/BaseP.frag");

	pShader->LoadShader("PlaneP", "Scenes/Scene3/PlaneP.vert", "Scenes/Scene3/PlaneP.frag");
	pShader->LoadShader("CrateP", "Scenes/Scene3/CrateP.vert", "Scenes/Scene3/CrateP.frag");
	pShader->LoadShader("HouseP", "Scenes/Scene3/HouseP.vert", "Scenes/Scene3/HouseP.frag");

	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	pTextures = new CTextures();
	pTextures->LoadTextureMipMap(0, "Scenes/Textures/CasaAtlas.jpg");
	pTextures->LoadTextureMipMap(1, "Scenes/Textures/grama.bmp");
	pTextures->LoadTextureMipMap(2, "Scenes/Textures/CRATE.BMP");

	pPlane.generatePlane(100.0f, 100.0f);
	pPlane.setupBuffers();

	pBox.generateCube(2.0f);
	pBox.setupBuffers();

	pHouse.push_back(CPrimitives());
	pHouse.back().generatePlane(8.0f, 8.0f);
	pHouse.back().setupBuffers();

	pHouse.push_back(CPrimitives());
	pHouse.back().generatePyramid(8.0f, 8.0f);
	pHouse.back().setupBuffers();

}

Scene3::~Scene3()
{
	pPlane.cleanup();
	pBox.cleanup();
	for (auto& house : pHouse)
	{
		house.cleanup();
	}	

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pShader)
	{
		delete pShader;
		pShader = NULL;
	}

	if (pGridAxis)
	{
		delete pGridAxis;
		pGridAxis = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pText)
	{
		delete pText;
		pText = NULL;
	}


}

void Scene3::DrawScene(float deltaTime)
{
	// Update timer (must be called at start of frame)
	pTimer->Update();

	// Clear the color and depth buffers
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create transformations 
	// Note: In a real application, you would typically calculate the projection and view matrices 
	// once per frame and pass them to your shaders
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), ASPECT_RATIO, Z_NEAR, Z_FAR);
	glm::mat4 view = pCamera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	// Draw the 3D grid
	pShader->Use("Grid");
	pShader->SetMat4("uProj", projection);
	pShader->SetMat4("uView", view);
	pShader->SetFloat("uGridSpacing", 1.0f);
	pShader->SetFloat("uFadeStart", 18.f);
	pShader->SetFloat("uFadeEnd", 100.f);
	pShader->SetVec3("uCamPos", pCamera->Position);
	pGridAxis->DrawGrid();
	// Draw the 3D axis
	pShader->Use("Axis");
	pShader->SetMat4("uProj", projection);
	pShader->SetMat4("uView", view);
	pGridAxis->DrawAxis();
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glDisable(GL_CULL_FACE);

	pShader->Use("HouseP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(0);
	pShader->SetInt("Texture0", 0);

	DrawHouse(glm::vec3(-8.0f, 0.0001f, -16.0f));

	glEnable(GL_CULL_FACE);

	// Desenha as caixas
	pShader->Use("CrateP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, 1.0f, 0.0f));

	pShader->SetMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(2);
	pShader->SetInt("Texture0", 0);

	pBox.render();

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	pShader->SetMat4("model", model);

	pBox.render();

	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.0f, 1.5f));

	pShader->SetMat4("model", model);

	pBox.render();

	// Desenha o plano
	pShader->Use("PlaneP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0f);
	pShader->SetMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(1);
	pShader->SetInt("Texture0", 0);

	pPlane.render();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw text on the screen (disable depth test and use orthographic projection)
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));

	pShader->Use("Text2D");
	pShader->SetMat4("projection", projection);
	pShader->SetVec3("textColor", 1.0f, 1.0f, 1.0f);

	pText->RenderText(std::format("CamPosition\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Position.x, pCamera->Position.y, pCamera->Position.z), 5.0f, 100.0f, 0.3f);
	pText->RenderText(std::format("CamFoward\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Front.x, pCamera->Front.y, pCamera->Front.z), 5.0f, 80.0f, 0.3f);

	pText->RenderText(std::format("FPS: {0:.0f}\tDeltatime: {1:.2f}ms\tTimer: {2:.2f}s", pTimer->GetFPS(), pTimer->GetDeltaTime(), pTimer->GetTime() / 1000), 5.0f, 10.0f, 0.3f);
	glEnable(GL_DEPTH_TEST);
}


void Scene3::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
	InputManager& input = InputManager::GetInstance();

	// Camera movement
	if (input.IsKeyPressed(GLFW_KEY_W))
		pCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (input.IsKeyPressed(GLFW_KEY_S))
		pCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (input.IsKeyPressed(GLFW_KEY_A))
		pCamera->ProcessKeyboard(LEFT, deltaTime);
	if (input.IsKeyPressed(GLFW_KEY_D))
		pCamera->ProcessKeyboard(RIGHT, deltaTime);
	if (input.IsKeyPressed(GLFW_KEY_E))
		pCamera->ProcessKeyboard(UP, deltaTime);
	if (input.IsKeyPressed(GLFW_KEY_Q))
		pCamera->ProcessKeyboard(DOWN, deltaTime);

	// Increase/decrease movement speed
	if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		pCamera->MovementSpeed = SPEED_MULTIPLIER;
	else
		pCamera->MovementSpeed = SPEED;

	// Process mouse movement
	glm::vec2 mouseDelta = input.GetMouseDelta();
	if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
	{
		pCamera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
	}

	// Process scroll
	float scrollOffset = input.GetScrollOffset();
	if (scrollOffset != 0.0f)
	{
		pCamera->ProcessMouseScroll(scrollOffset);
	}
}

void Scene3::DrawHouse(glm::vec3 position, float rotationAngle, glm::vec3 scale)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	// Chão
	glm::mat4 assistModel = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
	assistModel = glm::translate(assistModel, glm::vec3(-20.0f, 0.0f, 12.0f));

	pShader->SetInt("TextureType", 2); // Parede
	for (int i = 0; i <= 24; i = i + 8)
	{
		for (int j = 0; j <= 40; j = j + 8)
		{
			pShader->SetMat4("model", glm::translate(assistModel, glm::vec3(j, 0.0f, -i)));
			pHouse.front().render();
		}
	}

	// Laje
	pShader->SetInt("TextureType", 1); // Této
	for (int i = 0; i <= 24; i = i + 8)
	{
		for (int j = 0; j <= 40; j = j + 8)
		{
			pShader->SetMat4("model", glm::translate(assistModel, glm::vec3(j, 8.0f, -i)));
			pHouse.front().render();
		}
	}

	// Telhado
	assistModel = glm::translate(model, glm::vec3(0.0f, 9.9344f, 4.64952f));
	assistModel = glm::rotate(assistModel, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	assistModel = glm::scale(assistModel, glm::vec3(3.25f, 1.0f, 1.3422f));
	pShader->SetMat4("model", assistModel);

	pHouse.front().render();

	assistModel = glm::translate(model, glm::vec3(0.0f, 9.9344f, -4.64952f));
	assistModel = glm::rotate(assistModel, glm::radians(30.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	assistModel = glm::scale(assistModel, glm::vec3(3.25f, 1.0f, 1.3422f));
	pShader->SetMat4("model", assistModel);

	pHouse.front().render();

	assistModel = glm::translate(model, glm::vec3(12.0f, 8.0f, 0.0f));
	assistModel = glm::scale(assistModel, glm::vec3(0.0f, 0.57735f, 2.0f));
	pShader->SetMat4("model", assistModel);

	pHouse.back().render();

	assistModel = glm::translate(model, glm::vec3(-12.0f, 8.0f, 0.0f));
	assistModel = glm::scale(assistModel, glm::vec3(0.0f, 0.57735f, 2.0f));
	pShader->SetMat4("model", assistModel);

	pHouse.back().render();

	// Parede
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, 8.0f));
	model = glm::rotate(model, 1.5708f, glm::vec3(-1.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetInt("TextureType", 0); // Porta
	pHouse.front().render();

	model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetInt("TextureType", 2); // Parede
	pHouse.front().render();

	model = glm::translate(model, glm::vec3(0.0f, 16.0f, 0.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();

	model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();

	model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();

	model = glm::translate(model, glm::vec3(0.0f, -16.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetInt("TextureType", 3); // Janela
	pHouse.front().render();

	model = glm::translate(model, glm::vec3(-4.0f, 4.0f, 0.0f));
	model = glm::rotate(model, 1.5708f, glm::vec3(0.0f, 0.0f, -1.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();

	model = glm::translate(model, glm::vec3(0.0f, 24.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetInt("TextureType", 2); // Parede
	pHouse.front().render();

	model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();

	model = glm::translate(model, glm::vec3(0.0f, -24.0f, 0.0f));
	pShader->SetMat4("model", model);

	pHouse.front().render();
}