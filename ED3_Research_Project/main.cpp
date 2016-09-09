#include "Utilities.h"
#include "glm/gtc/type_ptr.hpp"
#include "ResourceManager.h"
#include "RenderingManager.h"
#include "EntityManager.h"
#include "Renderable.h"
#include "Registry.h"
#include "Transform.h"
#include "ShaderManager.h"
#include "XTime.h"
#include "GameWindow.h"
#include "Camera.h"
#include "GLError.h"

GameWindow window;

ResourceManager * resourceManager;
RenderingManager * renderingManager;
EntityManager * entityManager;
ShaderManager * shaderManager;
LightingManager * lightingManager;

unsigned int program[2];

unsigned int screenWidth = 1024;
unsigned int screenHeight = 720;

unsigned int perspectiveMatrixID, viewMatrixID, modelMatrixID;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 M;
glm::mat4 P;

Camera myCamera;
XTime timer;

void CleanUp();
void MainLoop();
void InitializeRegistry();

int main(int argc, char** argv){

	//Leak Detection
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetBreakAlloc(-1);

	//Setup the registry.
	InitializeRegistry();

	//Create the window
	window.Initalize(argc, argv);

	//Set the projection Matrix
	P = glm::mat4(1.0f);
	P = glm::perspectiveFov(1.0f / tan(60.0f*3.1415926f / 360.0f), (float)screenWidth, (float)screenHeight, 0.001f, 1000.0f);

	//Allocate all of the managers
	resourceManager = new ResourceManager();
	entityManager = new EntityManager();
	shaderManager = new ShaderManager();
	renderingManager = new RenderingManager();
	lightingManager = new LightingManager();

	//Add delegates between managers
	resourceManager->setEntityManager(entityManager);
	entityManager->setRenderingManager(renderingManager);
	entityManager->setLightingManager(lightingManager);
	renderingManager->setResourceManager(resourceManager);

	//Load Level
	resourceManager->LoadLevel("Level_1");

	//Create Shader Program and set main program.
	shaderManager->CreateProgram("Main", "Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	shaderManager->CreateProgram("animatedMain", "Shaders/AnimationVS.glsl", "Shaders/AnimationFS.glsl");
	program[0] = shaderManager->GetShader("Main");
	program[1] = shaderManager->GetShader("animatedMain");

	Registry::getInstance()->currentProgram = 3;

	//Get the matrix uniform locations from shaders.
	modelMatrixID = glGetUniformLocation(program[0], "model");
	perspectiveMatrixID = glGetUniformLocation(program[0], "projection");
	viewMatrixID = glGetUniformLocation(program[0], "view");

	modelMatrixID = glGetUniformLocation(program[1], "model");
	perspectiveMatrixID = glGetUniformLocation(program[1], "projection");
	viewMatrixID = glGetUniformLocation(program[1], "view");

	//Use Program
	glUseProgram(program[0]);

	//Intialize Camera
	myCamera.Initialize();

	//Activate Lights
	lightingManager->ActivateLights();

	//Sort Renderables before drawing.
	renderingManager->sortByMaterial();

	//Main Loop
	while (window.isRunning()) {
		MainLoop();
	}

	//Cleanup the window(GLFW)
	glfwTerminate();

	//Cleanup everything else.
	CleanUp();

	return 0;
}

void InitializeRegistry(){
	register_type(Renderable);
	register_type(Transform);
	register_type(Light);
}

void CleanUp() {
	delete resourceManager;
	delete entityManager;
	delete shaderManager;
	delete renderingManager;
	delete lightingManager;
	delete Registry::getInstance();
}

void MainLoop() {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glUseProgram(program[0]);

		myCamera.update(deltaTime);

		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(myCamera.getMatrix()));
		glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, glm::value_ptr(P));

		renderingManager->RenderAll();

		glUseProgram(0);

		glfwSwapBuffers(window.getWindow());

}