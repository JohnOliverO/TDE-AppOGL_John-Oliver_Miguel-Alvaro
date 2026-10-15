#include "Scene4.h"
#include <algorithm>


Scene4::Scene4()
{
	glEnable(GL_CULL_FACE);

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

	pShader->LoadShader("CasaShader", "Scenes/Assets/Casa/CasaShader.vert", "Scenes/Assets/Casa/CasaShader.frag");
	pShader->LoadShader("PlaneShader", "Scenes/Scene4/PlaneShader.vert", "Scenes/Scene4/PlaneShader.frag");

	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	pTextures = new CTextures();
	pTextures->LoadTextureMipMap(0, "Scenes/Assets/Casa/CasaTex.jpg");
	pTextures->LoadTextureMipMap(1, "Scenes/Textures/grass.BMP");
	
	HouseVertexCount = HouseLoader("Scenes/Assets/Casa/Casa.txt");

	pPlane.generatePlane(100.0f, 100.0f);
	pPlane.setupBuffers();

}

Scene4::~Scene4()
{
	DestroyHouse();

	pPlane.cleanup();

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

void Scene4::DrawScene(float deltaTime)
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

	glDisable(GL_CULL_FACE); // Desabilita o culling para que possamos ver o interior da casa

	pShader->Use("CasaShader"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(0);
	pShader->SetInt("Texture0", 0);

	DrawHouse(HouseVertexCount);

	glEnable(GL_CULL_FACE); // Reabilita o culling para os próximos objetos

	pShader->Use("PlaneShader"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	glm::mat4 modelPlane = glm::mat4(1.0f);

	pShader->SetMat4("model", modelPlane);

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


void Scene4::ProcessSceneInput(GLFWwindow* window, float deltaTime)
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

int Scene4::HouseLoader(std::string filename)
{
	std::ifstream file(filename);

	if(!file.is_open())
	{
		std::cout << "Error: Could not open file " << filename << std::endl;
		return 0;
	}

	std::vector<float> vertices;
	std::string line;

	float vertex;
	while(std::getline(file, line))
	{
		// Substitui todas as vírgulas por espaços para não quebrar o "stringstream"
		std::replace(line.begin(), line.end(), ',', ' ');

		std::stringstream ss(line);
		while(ss >> vertex)
		{
			vertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &HouseVAO);

	glGenBuffers(1, &HouseVBO);

	glBindVertexArray(HouseVAO);

	glBindBuffer(GL_ARRAY_BUFFER, HouseVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	int stride = 5 * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	file.close();

	return vertices.size() / 5; // Return the number of vertices (each vertex has 5 floats: 3 for position, 2 for texture coordinates)
}

void Scene4::DrawHouse(int vertexCount, glm::vec3 position, float rotationAngle, glm::vec3 scale)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
	pShader->SetMat4("model", model);
	glBindVertexArray(HouseVAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

void Scene4::DestroyHouse()
{
	glDeleteVertexArrays(1, &HouseVAO);
	glDeleteBuffers(1, &HouseVBO);
}