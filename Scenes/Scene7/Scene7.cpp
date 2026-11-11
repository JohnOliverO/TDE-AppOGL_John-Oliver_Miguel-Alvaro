#include "Scene7.h"


Scene7::Scene7()
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

	pShader->LoadShader("Skybox", "Scenes/Common/Skybox.vert", "Scenes/Common/Skybox.frag");
	pShader->LoadShader("Asset", "Scenes/Common/Assets.vert", "Scenes/Common/Assets.frag");
	pShader->LoadShader("Light", "Scenes/Common/Light.vert", "Scenes/Common/Light.frag");
	pShader->LoadShader("Blend", "Scenes/Common/Blend.vert", "Scenes/Common/Blend.frag");
	pShader->LoadShader("Fog", "Scenes/Common/Fog.vert", "Scenes/Common/Fog.frag");
	pShader->LoadShader("ColorLightFog", "Scenes/Common/ColorLightFog.vert", "Scenes/Common/ColorLightFog.frag");

	pShader->LoadShader("Primitives", "Scenes/Common/Primitives.vert", "Scenes/Common/Primitives.frag");

	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	//pTextures = new CTextures();
	
	pTerrain = NULL;
	pTerrain = new CAsset("Scenes/Scene7/Terrain.obj");

	pBoxes = NULL;
	pBoxes = new CAsset("Scenes/Scene7/Boxes.obj");

	pCube = NULL;
	pCube = new CAsset("Scenes/Scene7/Cube.obj");

	pPyramid = NULL; 
	pPyramid = new CAsset("Scenes/Scene7/Pyramid.obj");

	pHouse = NULL;
	pHouse = new CAsset("Scenes/Scene7/casa.obj");

	pGrass1 = NULL;
	pGrass1 = new CAsset("Scenes/Scene7/Grama1.obj");

	ambientStrength = 0.1f;

	// Inicia com a GUI desativada
	bool bActiveGUI;

	// Posição da fonte de luz (Point Light)
	lightPos = glm::vec3(0.0f, 10.0f, 20.0f);

	// Cria lâmpada (apenas para observação)
	pLamp.generateSphere(0.3f);
	pLamp.setupBuffers();

	// Velocidade de deslocamento da lâmpada
	fSpeed = 10.0f;

	// Intensidade do reflexo especular
	Shininess = 32.0f;

	// Cor da luz
	LightColor = glm::vec3(1.0, 1.0, 1.0);

	// Cor do assert
	assetColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	FogColor = glm::vec4(0.5, 0.5, 0.5, 1.0);
	FogDensity = 0.03;
	FogMode = 0;
	FogStart = 0.0;
	FogEnd = 100.0;
	FogEnable = true;

}

Scene7::~Scene7()
{
	//Destroi Lâmpada
	pLamp.cleanup();

	if (pGrass1) {
		delete pGrass1;
		pGrass1 = NULL;
	}

	if (pHouse) {
		delete pHouse;
		pHouse = NULL;
	}

	if (pTerrain) {
		delete pTerrain;
		pTerrain = NULL;
	}

	if (pBoxes) {
		delete pBoxes;
		pBoxes = NULL;
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

	if (pCube)
	{
		delete pCube;
		pCube = NULL;
	}

	if (pPyramid)
	{
		delete pPyramid;
		pPyramid = NULL;
	}


}

void Scene7::DrawScene(float deltaTime)
{
	// Update timer (must be called at start of frame)
	pTimer->Update();

	glm::vec4 clearColor = FogEnable ? FogColor : glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);

	// Clear the color and depth buffers
	glClearColor(clearColor.r , clearColor.g , clearColor.b , clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create transformations 
	// Note: In a real application, you would typically calculate the projection and view matrices 
	// once per frame and pass them to your shaders
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), ASPECT_RATIO, Z_NEAR, Z_FAR);
	glm::mat4 view = pCamera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Draw the 3D grid
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
	// Desenha a cena
	// Desenha a lâmpada
	pShader->Use("Primitives");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	pShader->SetMat4("model", model);

	pLamp.render();

	// Objetos com interação com Neblina
	pShader->Use("ColorLightFog");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	pShader->SetVec4("FogColor", FogColor);
	pShader->SetFloat("FogDensity", FogDensity);
	pShader->SetInt("FogMode", FogMode);
	pShader->SetFloat("FogStart", FogStart);
	pShader->SetFloat("FogEnd", FogEnd);
	pShader->SetVec3("CameraPos", pCamera->Position);
	pShader->SetBool("FogEnable", FogEnable);

	pShader->SetFloat("AmbientStrength", ambientStrength);
	pShader->SetVec3("LightPos", lightPos);
	pShader->SetVec3("ViewPos", pCamera->Position);
	pShader->SetVec3("LightColor", LightColor);
	pShader->SetFloat("Shininess", Shininess);

	/*
	std::string shaderToUse = FogEnable ? "Fog" : "Light";

	if (FogEnable == true)
	{
		// Objetos com interação com Neblina
		pShader->Use("Fog");
		pShader->SetMat4("projection", projection);
		pShader->SetMat4("view", view);

		pShader->SetVec4("FogColor", FogColor);
		pShader->SetFloat("FogDensity", FogDensity);
		pShader->SetInt("FogMode", FogMode);
		pShader->SetFloat("FogStart", FogStart);
		pShader->SetFloat("FogEnd", FogEnd);
		pShader->SetVec3("CameraPos", pCamera->Position);
		pShader->SetBool("FogEnable", FogEnable);
	}
	else
	{
		// Objetos com interação com luz
		pShader->Use("Light");
		pShader->SetMat4("projection", projection);
		pShader->SetMat4("view", view);

		pShader->SetFloat("AmbientStrength", ambientStrength);
		pShader->SetVec3("LightPos", lightPos);
		pShader->SetVec3("ViewPos", pCamera->Position);
		pShader->SetVec3("LightColor", LightColor);
		pShader->SetFloat("Shininess", Shininess);
	}
	*/
	// Desenha o terreno
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pTerrain->Draw(pShader->GetProgram("ColorLightFog"));

	// Desenha o cubo
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pCube->Draw(pShader->GetProgram("ColorLightFog"));

	// Desenha a casa
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-16.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pHouse->Draw(pShader->GetProgram("ColorLightFog"));

	// Desenha as caixas
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pBoxes->Draw(pShader->GetProgram("ColorLightFog"));

	/////////////////////////////////////////////////////
	//////////////// TRANSPARÊNCIA //////////////////////
	/////////////////////////////////////////////////////

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);

	// Desenha a piramide
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 5.0f));
	pShader->SetMat4("model", model);
	pPyramid->Draw(pShader->GetProgram("ColorLightFog"));

	// Desenha a grama
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
	pShader->SetMat4("model", model);
	pGrass1->Draw(pShader->GetProgram("ColorLightFog"));

	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);

	// Objetos sem interação com luz
	pShader->Use("Blend");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	// Passa a cor do asset + canal alpha
	pShader->SetVec4("assetColor", assetColor);

	// Desenha a piramide
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pPyramid->Draw(pShader->GetProgram("Blend"));

	// Desenha a grama
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pGrass1->Draw(pShader->GetProgram("Blend"));

	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	*/
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


void Scene7::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyJustPressed(GLFW_KEY_ENTER))
	{
		bActiveGUI = !bActiveGUI;
		input.SetCursorMode(bActiveGUI ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

		std::cout << "GUI mode: '" << bActiveGUI << "'" << std::endl;
	}

	if (bActiveGUI == false)
	{
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

		// Teclas de controle interativo de posição da luz
		if (input.IsKeyPressed(GLFW_KEY_UP))
			lightPos.z -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_DOWN))
			lightPos.z += fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_LEFT))
			lightPos.x -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_RIGHT))
			lightPos.x += fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_PAGE_DOWN))
			lightPos.y -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_PAGE_UP))
			lightPos.y += fSpeed * deltaTime;

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
	else
	{
		ShowGUI(); // Show Graphics User Interface
	}
}

void Scene7::ShowGUI()
{
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float;

	ImGui::Begin("Light Options:");

	ImGui::SliderFloat("Ambient Strenght", &ambientStrength, 0.0f, 1.0f, "%.2f");
	ImGui::SliderFloat("Shininess", &Shininess, 0.1f, 254.0f, "%.2f");
	ImGui::ColorEdit4("Light Color", (float*)&LightColor, flags);
	ImGui::NewLine();
	ImGui::ColorEdit4("Asset Color", (float*)&assetColor, flags);
	ImGui::End();

	ImGui::Begin("Fog Options:");
	ImGui::Text("Fog Mode:");
	ImGui::RadioButton("Linear", &FogMode, 0); ImGui::SameLine();
	ImGui::RadioButton("Exponential", &FogMode, 1); ImGui::SameLine();
	ImGui::RadioButton("Quadratic", &FogMode, 2); ImGui::SameLine();
	ImGui::ColorEdit4("Fog Color", (float*)&FogColor, flags);
	ImGui::SliderFloat("Fog Density", &FogDensity, 0.0f, 1.0f, "%.2f");
	ImGui::SliderFloat("Fog Start", &FogStart, 0.0f, 1000.0f, "%.2f");
	ImGui::SliderFloat("Fog End", &FogEnd, 0.0f, 1000.0f, "%.2f");
	ImGui::Checkbox("Fog Enable", &FogEnable);
	ImGui::End();
}