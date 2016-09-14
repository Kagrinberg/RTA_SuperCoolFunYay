#include <Windows.h>

#include "GLError.h"

#include "ResourceManager.h"
#include "RenderingManager.h"
#include "EntityManager.h"
#include "ShaderManager.h"

#include "Renderable.h"
#include "Registry.h"
#include "Transform.h"

#include "GameWindow.h"
#include "Camera.h"
#include "Skybox.h"

GameWindow window;

ResourceManager * resourceManager;
RenderingManager * renderingManager;
EntityManager * entityManager;
ShaderManager * shaderManager;
LightingManager * lightingManager;

unsigned int Main, AnimatedMain, SkyBox;
unsigned int uboMatrices;

unsigned int perspectiveMatrixID, viewMatrixID, viewPosID, modelMatrixID;

unsigned int screenWidth = 1024;
unsigned int screenHeight = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix;

Camera myCamera;
Skybox skybox;

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
	shaderManager->CreateProgram("AnimatedMain", "Shaders/AnimationVS.glsl", "Shaders/AnimationFS.glsl");
	shaderManager->CreateProgram("SkyBox", "Shaders/SkyBoxVS.glsl", "Shaders/SkyBoxFS.glsl");
	Main = shaderManager->GetShader("Main");
	AnimatedMain = shaderManager->GetShader("AnimatedMain");
	SkyBox = shaderManager->GetShader("SkyBox");

	skybox.LoadSkybox();


	////Set the projection Matrix in the Uniform buffer Object
	projectionMatrix = glm::perspectiveFov(1.0f / tan(60.0f*3.1415926f / 360.0f), (float)screenWidth, (float)screenHeight, 0.001f, 1000.0f);

	//glUseProgram(AnimatedMain);
	lightingManager->ActivateLights(AnimatedMain);

	//Activate Lights for Main and Animated Main
	lightingManager->ActivateLights(Main);


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

		myCamera.update(deltaTime);

		renderingManager->RenderAll(myCamera, lightingManager);

		skybox.Render(SkyBox, glm::mat4(glm::mat3(myCamera.getMatrix())), projectionMatrix);

		check_gl_error();

		glfwSwapBuffers(window.getWindow());

}