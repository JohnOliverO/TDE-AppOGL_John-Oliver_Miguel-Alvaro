#include "Scene1.h"


Scene1::Scene1()
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
	pShader->LoadShader("TextureCubeP", "Scenes/Scene1/TextureCubeP.vert", "Scenes/Scene1/TextureCubeP.frag");
	pShader->LoadShader("TextureSphereP", "Scenes/Scene1/TextureSphereP.vert", "Scenes/Scene1/TextureSphereP.frag");
	pShader->LoadShader("SnowPlane", "Scenes/Scene1/SnowPlane.vert", "Scenes/Scene1/SnowPlane.frag");
	pShader->LoadShader("SnowMan", "Scenes/Scene1/SnowMan.vert", "Scenes/Scene1/SnowMan.frag");
	pShader->LoadShader("Tree", "Scenes/Scene1/Tree.vert", "Scenes/Scene1/Tree.frag");
	pShader->LoadShader("Circle", "Scenes/Scene1/Circle.vert", "Scenes/Scene1/Circle.frag");


	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cria o gerenciador de texturas
	pTextures = NULL;
	pTextures = new CTextures();
	pTextures->LoadTextureAnisotropic(0, "Scenes/Textures/snow.jpg");

	// Cria as primitivas
	cube.generateCube(2.0);
	cube.setupBuffers();

	sphere.generateSphere(1.0);
	sphere.setupBuffers();

	cylinder.generateCylinder(0.5f, 3.0f);
	cylinder.setupBuffers();

	cone.generateCone(2.0f, 3.0f);
	cone.setupBuffers();

	plane.generatePlane(50.0f,50.0f);
	plane.setupBuffers();
	
}

Scene1::~Scene1()
{
	cube.cleanup();
	sphere.cleanup();
	cylinder.cleanup();
	cone.cleanup();
	plane.cleanup();

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

void Scene1::DrawScene(float deltaTime)
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
	/*
	// Renderiza o Cubo

	pShader->Use("TextureCubeP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetVec4("Color",glm::vec4(1.0,1.0,1.0,1.0));

	cube.render();

	// Renderiza a Esfera

	pShader->Use("TextureSphereP");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetVec4("Color", glm::vec4(0.85, 0.54, 0.92, 1.0));

	sphere.render();
	*/
	// Renderiza o Solo de Neve
	pShader->Use("SnowPlane");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	pShader->SetMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	pTextures->ApplyTexture(0);
	pShader->SetInt("Texture0", 0);

	plane.render();

	// Renderiza as Arvore
	pShader->Use("Tree");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	DrawTree(glm::vec3(0.0, 0.0, -5.0), glm::vec3(1.0, 1.0, 1.0));
	DrawTree(glm::vec3(-2.0, 0.0, -5.0), glm::vec3(1.0, 2.0, 1.0));
	DrawTree(glm::vec3(5.0, 0.0, 5.0), glm::vec3(2.0, 3.0, 2.0));

	// Renderiza o Boneco de Neve
	pShader->Use("SnowMan");
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	if(danceDirection){
		danceAngle += 10 * deltaTime;
		if (danceAngle > 90) {
			danceDirection = !danceDirection;
		}
	} else {
		danceAngle -= 10 * deltaTime;
		if (danceAngle < 0) {
			danceDirection = !danceDirection;
		}
	}

	DrawSnowMan(glm::vec3(0.0, 0.0, 5.0), glm::vec3(1.0, 1.0, 1.0), danceAngle);














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


void Scene1::ProcessSceneInput(GLFWwindow* window, float deltaTime)
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

void Scene1::DrawTree(glm::vec3 pos, glm::vec3 size)
{
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, pos);
	model = glm::scale(model, size);

	// Renderiza Tronco (Cilindro)
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	pShader->SetMat4("model", model);

	pShader->SetVec4("Color", glm::vec4(0.58, 0.39, 0.17, 1.0));

	cylinder.render();

	// Renderiza Folha1 (Cone)
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetVec4("Color", glm::vec4(0.0, 0.5, 0.0, 1.0));

	cone.render();

	// Renderiza Folha2 (Cone)
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	pShader->SetMat4("model", model);

	pShader->SetVec4("Color", glm::vec4(0.0, 0.5, 0.0, 1.0));

	cone.render();
}

void Scene1::DrawSnowMan(glm::vec3 pos, glm::vec3 size, float danceAngle)
{
	glm::mat4 model = glm::mat4(1.0);

	model = glm::translate(model, pos);
	model = glm::scale(model, size);
	model = glm::rotate(model, glm::radians(danceAngle-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Renderiza Base (Esfera)
	model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
	sphere.render();

	// Renderiza Corpo (Esfera)
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
	sphere.render();
	DrawSnowManBodyDitals(model);

	// Renderiza Cabeça (Esfera)
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
	sphere.render();
	DrawSnowManHeadDitals(model);
	DrawSnowManHat(model);
}

void Scene1::DrawSnowManBodyDitals(glm::mat4 model)
{
	model = glm::translate(model, glm::vec3(0.0, -0.25, 0.9));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	sphere.render();
	model = glm::translate(model, glm::vec3(0.0, 4.0, 0.0));
	pShader->SetMat4("model", model);
	sphere.render();
}

void Scene1::DrawSnowManHeadDitals(glm::mat4 model)
{
	model = glm::translate(model, glm::vec3(-0.4, 0.25, 0.9));
	model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	sphere.render();
	model = glm::translate(model, glm::vec3(4.0, 0.0, 0.0));
	pShader->SetMat4("model", model);
	sphere.render();
	model = glm::translate(model, glm::vec3(-2.0, -2.0, 2.0));
	model = glm::scale(model, glm::vec3(0.50f, 0.50f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(1.0, 0.5, 0.0, 1.0));
	cone.render();
}

void Scene1::DrawSnowManHat(glm::mat4 model)
{
	model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
	model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
	pShader->SetMat4("model", model);
	pShader->SetVec4("Color", glm::vec4(0.5, 0.20, 1.0, 1.0));
	cylinder.render();
	model = glm::translate(model, glm::vec3(0.0, 0.25, 0.0));
	model = glm::scale(model, glm::vec3(1.5f, 0.20f, 1.5f));
	pShader->SetMat4("model", model);
	cylinder.render();
}

// forma adicional para entrega de trabalho
void Scene1::CreateCircle(float radius, int segments)
{
	// Aloca o número de segmentos do círculo ao cabesalho da classe para uso posterior
	this->numSegments = segments;

	std::vector<float> vertices;

	// Calcula o angulo entre cada segmento do círculo
	float angle = 2.0f * M_PI / segments;

	// Adiciona o centro do círculo como o primeiro vértice
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	// Adiciona os vértices ao redor do círculo
	for (int i = 0; i <= segments; i++)
	{
		float x = radius * cos(i * angle);
		float y = radius * sin(i * angle);

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);
	}

	// Cria o VAO
	glGenVertexArrays(1, &circleVAO);
	// Cria o VBO
	glGenBuffers(1, &circleVBO);

	// Ativa o VAO
	glBindVertexArray(circleVAO);

	// Ativa o VBO e aloca os dados do objeto nele(vai para a GPU)
	// Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Define os atributos de cada vértice(VAO)
	// Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	// Posição do vértice (ID=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// Cor do vértice (ID=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Desativa o VAO (evita conflito com outros VAOs)
	glBindVertexArray(0);
}
void Scene1::DrawCircle()
{
	// Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(circleVAO);

	// Desenha o objeto
	// Renderizamos o centro + todos os vértices dos segmentos + o ponto final para fechar o círculo
	// Usamos GL_TRIANGLE_FAN para conectar todos ao redor do centro
	glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

	// Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene1::DestroyCircle()
{
	glDeleteVertexArrays(1, &circleVAO);
	glDeleteBuffers(1, &circleVBO);
}