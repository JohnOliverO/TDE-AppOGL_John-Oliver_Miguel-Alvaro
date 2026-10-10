#include "Scene2.h"
#include <stack>


Scene2::Scene2()
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
	pShader->LoadShader("StackP", "Scenes/Scene2/StackP.vert", "Scenes/Scene2/StackP.frag");

	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	//pTextures = new CTextures();

	std::vector<glm::vec4> initialColors = {
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
		glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)  // Orange
	};

	stackInfo0.Shape.generateCube(1.0);
	stackInfo0.Shape.setupBuffers();
	stackInfo0.positions = glm::vec3(-2.0f, 0.0f, 0.0f);

	stackInfo1.Shape.generateCube(1.0);
	stackInfo1.Shape.setupBuffers();
	stackInfo1.positions = glm::vec3(-1.0f, 0.0f, 0.0f);

	stackInfo2.Shape.generateCube(1.0);
	stackInfo2.Shape.setupBuffers();
	stackInfo2.positions = glm::vec3(0.0f, 0.0f, 0.0f);

	stackInfo3.Shape.generateCube(1.0);
	stackInfo3.Shape.setupBuffers();
	stackInfo3.positions = glm::vec3(1.0f, 0.0f, 0.0f);

	stackInfo4.Shape.generateCube(1.0);
	stackInfo4.Shape.setupBuffers();
	stackInfo4.positions = glm::vec3(2.0f, 0.0f, 0.0f);
}

Scene2::~Scene2()
{
	stackInfo0.Shape.cleanup();
	stackInfo1.Shape.cleanup();
	stackInfo2.Shape.cleanup();
	stackInfo3.Shape.cleanup();
	stackInfo4.Shape.cleanup();

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

void Scene2::DrawScene(float deltaTime)
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Logica para acrescentar cubos nas pilhas
	if (stackInfo0.cubeColors.size() == stackInfo0.stackHeight)
		fullStacks[0] = true;
	if (stackInfo1.cubeColors.size() == stackInfo1.stackHeight)
		fullStacks[1] = true;
	if (stackInfo2.cubeColors.size() == stackInfo2.stackHeight)
		fullStacks[2] = true;
	if (stackInfo3.cubeColors.size() == stackInfo3.stackHeight)
		fullStacks[3] = true;
	if (stackInfo4.cubeColors.size() == stackInfo4.stackHeight)
		fullStacks[4] = true;

	if (fullStacks[0] == true && fullStacks[1] == true && fullStacks[2] == true && fullStacks[3] == true && fullStacks[4] == true)
	{
		// Todas as stacks estão cheias.
		// Logica para resetar a pilha de cubos

		// Acumula o tempo que passou desde o último frame (convertendo ms para segundos)
		cubeSpawnTimer += (pTimer->GetDeltaTime() / 1000.0f);

		// Se passou 1 segundo ou mais
		if (cubeSpawnTimer >= 1.0f)
		{
			cubeSpawnTimer -= 1.0f; // Mantém o resto para não perder precisão técnica

			stackInfo0.cubeColors = std::stack<glm::vec4>();
			stackInfo1.cubeColors = std::stack<glm::vec4>();
			stackInfo2.cubeColors = std::stack<glm::vec4>();
			stackInfo3.cubeColors = std::stack<glm::vec4>();
			stackInfo4.cubeColors = std::stack<glm::vec4>();

			fullStacks = { false, false, false, false, false };
		}
	}
	else
	{
		// Acumula o tempo que passou desde o último frame (convertendo ms para segundos)
		cubeSpawnTimer += (pTimer->GetDeltaTime() / 1000.0f);

		// Se passou 1 segundo ou mais, adiciona o cubo e desconta 1.0s do temporizador
		if (cubeSpawnTimer >= 1.0f)
		{
			cubeSpawnTimer -= 1.0f; // Mantém o resto para não perder precisão técnica

			// Gera uma cor aleatória para o cubo
			glm::vec4 cubeColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1.0f);

			bool addedCube = false;
			while (!addedCube) {
				int randomStack = rand() % 5; // Seleciona pilha aleatória de 0 a 4
				if (!fullStacks[randomStack])
				{
					switch (randomStack)
					{
					case 0: stackInfo0.cubeColors.push(cubeColor); break;
					case 1: stackInfo1.cubeColors.push(cubeColor); break;
					case 2: stackInfo2.cubeColors.push(cubeColor); break;
					case 3: stackInfo3.cubeColors.push(cubeColor); break;
					case 4: stackInfo4.cubeColors.push(cubeColor); break;
					}
					addedCube = true;
				}
			}
		}
	}




	// Renderiza a pilha de cubos
	pShader->Use("StackP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	DrawStackOfCubes(stackInfo0);
	DrawStackOfCubes(stackInfo1);
	DrawStackOfCubes(stackInfo2);
	DrawStackOfCubes(stackInfo3);
	DrawStackOfCubes(stackInfo4);
	










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


void Scene2::ProcessSceneInput(GLFWwindow* window, float deltaTime)
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

void Scene2::DrawStackOfCubes(StackInfo& stackInfo)
{
	glm::mat4 model = glm::mat4(1.0f);

	// Fazemos uma cópia da pilha de cores para poder desempilhar sem perder os dados originais
	std::stack<glm::vec4> tempColors = stackInfo.cubeColors;
	int totalCubes = tempColors.size();

	// Loop para desenhar cada cubo da pilha atual
	for (int i = 0; i < totalCubes; ++i)
	{
		// Calcula a posição de cada bloco empilhado no eixo Y (i * 1.0f pressupõe cubos de tamanho 1.0)
		glm::vec3 cubePosition = stackInfo.positions + glm::vec3(0.0f, i * 1.0f, 0.0f);

		model = glm::translate(glm::mat4(1.0f), cubePosition);
		pShader->SetMat4("model", model);

		// Pega a cor do topo da pilha temporária e passa para o shader
		glm::vec4 color = tempColors.top();
		pShader->SetVec4("Color", color);
		tempColors.pop();

		// Renderiza o cubo
		stackInfo.Shape.render();
	}
}