#include "Scene0.h"


Scene0::Scene0()
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

	pShader->LoadShader("SolidColor", "Scenes/Scene0/Solid.vert", "Scenes/Scene0/Solid.frag");

	pShader->LoadShader("Texture", "Scenes/Scene0/Texture.vert", "Scenes/Scene0/Texture.frag");


	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cor do Framebuffer (Cor de fundo)
	vFramebufferColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);

	// Entra no modo GUI
	bActiveGUI = false;

	// Cria o gerenciador de texturas
	pTextures = NULL;
	pTextures = new CTextures();
	pTextures->LoadTextureAnisotropic(0, "Scenes/Textures/cx2.BMP");
	pTextures->LoadTextureAnisotropic(1, "Scenes/Textures/grass.BMP");

	//Inicializar as formas geométricas da cena
	//Aloca as formas da VRAM
	CreateTriangle();
	CreateSquare();
	CreateCube();
	CreateBox(3.0f, 2.0f, 1.0f);
	//CreateTriangularPyramid(2.0f, 3.0f);
	CreateSquarePyramid(2.0f, 3.0f);
	CreatePlane();

	fPosX = 0.0f;
	fPosY = 0.0f;
	fPosZ = 0.0f;
	fSpeed = 10.0f;

	vScale = glm::vec3(1.0f, 1.0f, 1.0f);

	fRotSpeed = 50.0f;
	bRot = true;

	vCubeColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	bCubeColor = false;
}

Scene0::~Scene0()
{
	//Desaloca o triangulo da VRAM
	DestroyTriangle();
	DestroySquare();
	DestroyCube();
	DestroyBox();
	//DestroyTriangularPyramid();
	DestroySquarePyramid();
	DestroyPlane();

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

void Scene0::DrawScene(float deltaTime)
{
	// Update timer (must be called at start of frame)
	pTimer->Update();



	// Clear the color and depth buffers
	glClearColor(vFramebufferColor.r, vFramebufferColor.g, vFramebufferColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create transformations 
	// Note: In a real application, you would typically calculate the projection and view matrices 
	// once per frame and pass them to your shaders
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), ASPECT_RATIO, Z_NEAR, Z_FAR);
	glm::mat4 view = pCamera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*// Draw the 3D grid
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

	// Desenha o cubo
	pShader->Use("Texture"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);

	model = glm::translate(model, glm::vec3(fPosX, fPosY, fPosZ));
	model = glm::scale(model, vScale);

	pShader->SetMat4("model", model);

	if (bCubeColor == true) {
		pShader->SetVec4("vCubeColor", vCubeColor);
	}

	//Seta a textura a ser aplicada
	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(0);
	pShader->SetInt("Texture0",0);

	//pShader->SetBool("bSolidColor", bCubeColor);
	//pShader->SetVec4("vCubeColor", vCubeColor);

	DrawCube();

	//pShader->SetVec3("solidColor", glm::vec3(1.0, 0.0, 1.0)); // Define cor

	//DrawTriangle();
	//DrawSquare();
	//DrawBox();
	//DrawTriangularPyramid();

	// Desenha o triangulo
	pShader->Use("SolidColor"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
	if (bRot == true) {
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * fRotSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	pShader->SetMat4("model", model);

	pShader->SetBool("bSolidColor", false);

	DrawSquarePyramid();

	//pShader->SetVec3("solidColor", glm::vec3(1.0, 0.0, 1.0)); // Define cor

	// Desenha o Plane
	pShader->Use("Texture"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	
	pShader->SetMat4("model", model);

	//Seta a textura a ser aplicada no Plane
	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(1);
	pShader->SetInt("Texture0", 0);

	DrawPlane();

















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
	pText->RenderText(std::format("[ENTER] GUI mode: {0}", bActiveGUI ? "true" : "false"), 5.0f, 60.0f, 0.3f);

	pText->RenderText(std::format("FPS: {0:.0f}\tDeltatime: {1:.2f}ms\tTimer: {2:.2f}s", pTimer->GetFPS(), pTimer->GetDeltaTime(), pTimer->GetTime() / 1000), 5.0f, 10.0f, 0.3f);
	glEnable(GL_DEPTH_TEST);
}


void Scene0::ProcessSceneInput(GLFWwindow* window, float deltaTime)
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

		//Teclas de controle interativo de posição do cubo
		if (input.IsKeyPressed(GLFW_KEY_UP))
			fPosZ -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_DOWN))
			fPosZ += fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_LEFT))
			fPosX -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_RIGHT))
			fPosX += fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_PAGE_DOWN))
			fPosY -= fSpeed * deltaTime;
		if (input.IsKeyPressed(GLFW_KEY_PAGE_UP))
			fPosY += fSpeed * deltaTime;

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

void Scene0::ShowGUI()
{
	std::string sRot = bRot ? "Rotation ON" : "Rotation OFF";
	std::string sColor = bCubeColor ? "Solid Color ON" : "Solid Color OFF";

	ImGui::Begin("Scene Options:");
	ImGui::Text("Framebuffer Color:");
	ImGui::ColorEdit4(" ", (float*)&vFramebufferColor);
	if (ImGui::Button("Reset Color"))
		vFramebufferColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);

	ImGui::End();

	ImGui::Begin("Cube Options:");

	ImGui::SliderFloat3("Scale", (float*)&vScale, 0.01f, 10.0f, "%.2f");

	ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float;
	ImGui::ColorEdit4("Cube Color", (float*)&vCubeColor, flags);

	if (ImGui::Button(sColor.c_str()))
		bCubeColor = !bCubeColor;

	if (ImGui::Button("Reset Cube")) {
		vScale = glm::vec3(1.0f, 1.0f, 1.0f);
		bCubeColor = false;
		fPosX = 0.0f;
		fPosY = 0.0f;
		fPosZ = 0.0f;
		fSpeed = 10.0f;
	}

	ImGui::End();

	ImGui::Begin("Triangle Options:");

	ImGui::InputFloat("Rotation Velocity:", &fRotSpeed, 0.0f, 100.0f, "%.2f");

	if (ImGui::Button(sRot.c_str()))
		bRot = !bRot;
	if (ImGui::Button("Reset Triangle")) {
		fRotSpeed = 10.0;
		bRot = false;
	}

	ImGui::End();
}

void Scene0::CreateTriangle()
{
	float vertices[] = {
		//	X		Y		Z		R     G     B
			-1.0f,	0.0f,	0.0f,	1.0f, 0.0f, 1.0f, //v0
			1.0f,	0.0f,	0.0f,	0.0f, 1.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &triangleVAO);
	//Cria o VBO
	glGenBuffers(1, &triangleVBO);

	//Ativa o VAO
	glBindVertexArray(triangleVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);

}

void Scene0::DrawTriangle()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(triangleVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyTriangle()
{
	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteBuffers(1, &triangleVBO);
}

void Scene0::CreateSquare()
{
	float vertices[] = {
		//	X		Y		Z		R     G     B
			-1.0f,	1.0f,	0.0f,	0.5f, 0.5f, 0.5f, //v0
			-1.0f,	-1.0f,	0.0f,	1.0f, 1.0f, 1.0f, //v1
			1.0f,	-1.0f,	0.0f,	0.5f, 0.5f, 0.5f, //v2

			1.0f,	-1.0f,	0.0f,	0.5f, 0.5f, 0.5f, //v0
			1.0f,	1.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v1
			-1.0f,	1.0f,	0.0f,	0.5f, 0.5f, 0.5f, //v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &squareVAO);
	//Cria o VBO
	glGenBuffers(1, &squareVBO);

	//Ativa o VAO
	glBindVertexArray(squareVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawSquare()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(squareVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroySquare()
{
	glDeleteVertexArrays(1, &squareVAO);
	glDeleteBuffers(1, &squareVBO);
}

void Scene0::CreateCube()
{
	float vertices[] = {
		//positions					//colors			//Coord. Text
		//	  X		 Y		 Z		 R     G     B       S	   T
		//	Face0
			-1.0f,	1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v0
			-1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v1
			1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v2
			
			1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v0
			1.0f,	1.0f,	1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,//v1
			-1.0f,	1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v2

		//	Face1
			1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v0
			1.0f,	-1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v1
			1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v2

			1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v0
			1.0f,	1.0f,	1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,//v1
			1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v2

		//	Face2
			1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v0
			-1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v1
			-1.0f,	1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v2

			-1.0f,	1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v0
			1.0f,	1.0f,	1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,//v1
			1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v2

		//	Face3
			1.0f,	-1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v0
			-1.0f,	-1.0f,	-1.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,//v1
			-1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v2

			-1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v0
			1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v1
			1.0f,	-1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,//v2

		//	Face4
			-1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v0
			-1.0f,	-1.0f,	-1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,//v1
			-1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v2

			-1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v0
			-1.0f,	1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v1
			-1.0f,	1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v2

		//	Face5
			-1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v0
			-1.0f,	-1.0f,	-1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,//v1
			1.0f,	-1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v2

			1.0f,	-1.0f,	-1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,//v0
			1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,//v1
			-1.0f,	-1.0f,	1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,//v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &cubeVAO);
	//Cria o VBO
	glGenBuffers(1, &cubeVBO);

	//Ativa o VAO
	glBindVertexArray(cubeVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 8 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Cor do vértice (ID=2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawCube()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(cubeVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyCube()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
}

void Scene0::CreateBox(float xSize, float ySize, float zSize)
{
	float vertices[] = {
		//	X		Y		Z		R     G     B
		//	Face0
			-xSize,	ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v1
			xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2

			xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	ySize,	zSize,	0.0f, 0.0f, 0.0f, //v1
			-xSize,	ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2

		//	Face1
			xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	-ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v1
			xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

			xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	ySize,	zSize,	0.0f, 0.0f, 0.0f, //v1
			xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2

		//	Face2
			xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v1
			-xSize,	ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2

			-xSize,	ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	ySize,	zSize,	0.0f, 0.0f, 0.0f, //v1
			xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

		//	Face3
			xSize,	-ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	-ySize,	-zSize,	1.0f, 1.0f, 1.0f, //v1
			-xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

			-xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v1
			xSize,	-ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

		//	Face4
			-xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	-ySize,	-zSize,	1.0f, 1.0f, 1.0f, //v1
			-xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2

			-xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	ySize,	zSize,	0.5f, 0.5f, 0.5f, //v1
			-xSize,	ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

		//	Face5
			-xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v0
			-xSize,	-ySize,	-zSize,	1.0f, 1.0f, 1.0f, //v1
			xSize,	-ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v2

			xSize,	-ySize,	-zSize,	0.5f, 0.5f, 0.5f, //v0
			xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v1
			-xSize,	-ySize,	zSize,	0.5f, 0.5f, 0.5f, //v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &boxVAO);
	//Cria o VBO
	glGenBuffers(1, &boxVBO);

	//Ativa o VAO
	glBindVertexArray(boxVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawBox()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(boxVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyBox()
{
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteBuffers(1, &boxVBO);
}
/*
void Scene0::CreateTriangularPyramid()
{
	float vertices[] = {
		//	X		Y		Z		R     G     B
			-1.0f,	0.0f,	1.0f,	1.0f, 0.0f, 1.0f, //v0
			1.0f,	0.0f,	1.0f,	0.0f, 1.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			1.0f,	0.0f,	1.0f,	0.0f, 0.0f, 1.0f, //v0
			1.0f,	0.0f,	-1.0f,	0.0f, 0.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			1.0f,	0.0f,	-1.0f,	0.0f, 1.0f, 0.0f, //v0
			-1.0f,	0.0f,	-1.0f,	0.0f, 1.0f, 0.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			-1.0f,	0.0f,	-1.0f,	1.0f, 1.0f, 1.0f, //v0
			-1.0f,	0.0f,	1.0f,	1.0f, 0.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f //v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &triangularPyramidVAO);
	//Cria o VBO
	glGenBuffers(1, &triangularPyramidVBO);

	//Ativa o VAO
	glBindVertexArray(triangularPyramidVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, triangularPyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);

}

void Scene0::DrawTriangularPyramid()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(triangularPyramidVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyTriangularPyramid()
{
	glDeleteVertexArrays(1, &triangularPyramidVAO);
	glDeleteBuffers(1, &triangularPyramidVBO);
}
*/

void Scene0::CreateSquarePyramid(float baseSize, float height)
{
	float vertices[] = {
		//	X		Y		Z		R     G     B
			-1.0f,	0.0f,	1.0f,	1.0f, 0.0f, 1.0f, //v0
			1.0f,	0.0f,	1.0f,	0.0f, 1.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			1.0f,	0.0f,	1.0f,	0.0f, 0.0f, 1.0f, //v0
			1.0f,	0.0f,	-1.0f,	0.0f, 0.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			1.0f,	0.0f,	-1.0f,	0.0f, 1.0f, 0.0f, //v0
			-1.0f,	0.0f,	-1.0f,	0.0f, 1.0f, 0.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f, //v2

			-1.0f,	0.0f,	-1.0f,	1.0f, 1.0f, 1.0f, //v0
			-1.0f,	0.0f,	1.0f,	1.0f, 0.0f, 1.0f, //v1
			0.0f,	2.0f,	0.0f,	0.0f, 0.0f, 0.0f //v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &squarePyramidVAO);
	//Cria o VBO
	glGenBuffers(1, &squarePyramidVBO);

	//Ativa o VAO
	glBindVertexArray(squarePyramidVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, squarePyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);

}

void Scene0::DrawSquarePyramid()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(squarePyramidVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 12);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroySquarePyramid()
{
	glDeleteVertexArrays(1, &squarePyramidVAO);
	glDeleteBuffers(1, &squarePyramidVBO);
}

void Scene0::CreatePlane()
{
	float vertices[] = {
		//positions					//colors			//Coord. Text
		//	  X		 Y		 Z		 R     G     B       S	   T
		//	Face0
			-50.0f,	0.0f,	-50.0f,	0.5f, 0.5f, 0.5f,	0.0f, 5.0f,//v0
			-50.0f,	0.0f,	50.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,//v1
			50.0f,	0.0f,	50.0f,	0.5f, 0.5f, 0.5f,	5.0f, 0.0f,//v2

			50.0f,	0.0f,	50.0f,	0.5f, 0.5f, 0.5f,	5.0f, 0.0f,//v0
			50.0f,	0.0f,	-50.0f,	0.0f, 0.0f, 0.0f,	5.0f, 5.0f,//v1
			-50.0f,	0.0f,	-50.0f,	0.5f, 0.5f, 0.5f,	0.0f, 5.0f,//v2
	};

	//Cria o VAO
	glGenVertexArrays(1, &planeVAO);
	//Cria o VBO
	glGenBuffers(1, &planeVBO);

	//Ativa o VAO
	glBindVertexArray(planeVAO);

	//Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	//Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define os atributos de cada vértice(VAO)
	//Diz para a GPU como interpretar os dados do VBO
	int stride = 8 * sizeof(float);

	//Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Cor do vértice (ID=2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawPlane()
{
	//Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(planeVAO);

	//Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyPlane()
{
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
}