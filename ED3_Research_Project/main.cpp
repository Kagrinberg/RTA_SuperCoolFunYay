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

unsigned int Main, AnimatedMain;
unsigned int uboMatrices;

unsigned int perspectiveMatrixID, viewMatrixID, viewPosID, modelMatrixID;


unsigned int screenWidth = 1024;
unsigned int screenHeight = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix;

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
	Main = shaderManager->GetShader("Main");
	AnimatedMain = shaderManager->GetShader("AnimatedMain");

	//perspectiveMatrixID = glGetUniformLocation(Main, "projection");
	//viewMatrixID = glGetUniformLocation(Main, "view");
	//viewPosID = glGetUniformLocation(Main, "viewPos");
	//modelMatrixID = glGetUniformLocation(Main, "model");


	////perspectiveMatrixID = glGetUniformLocation(AnimatedMain, "projection");
	////viewMatrixID = glGetUniformLocation(AnimatedMain, "view");
	//viewPosID = glGetUniformLocation(AnimatedMain, "viewPos");
	////modelMatrixID = glGetUniformLocation(AnimatedMain, "model");


	////Get block inidcies
	//unsigned int uniformBlockIndexMain = glGetUniformBlockIndex(Main, "Matrices");
	////unsigned int uniformBlockIndexAnimatedMain = glGetUniformBlockIndex(AnimatedMain, "Matrices");

	////Link them
	//glUniformBlockBinding(Main, uniformBlockIndexMain, 0);
	////glUniformBlockBinding(AnimatedMain, uniformBlockIndexAnimatedMain, 0);

	////Create the uniform buffer object
	//glGenBuffers(1, &uboMatrices);
	//glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	//glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	////Set the buffer range
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	////Set the projection Matrix in the Uniform buffer Object
	projectionMatrix = glm::perspectiveFov(1.0f / tan(60.0f*3.1415926f / 360.0f), (float)screenWidth, (float)screenHeight, 0.001f, 1000.0f);
	//glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	////Intialize Camera
	//myCamera.Initialize();



	////glUseProgram(AnimatedMain);
	//lightingManager->ActivateLights(AnimatedMain);

	////Activate Lights for Main and Animated Main
	//lightingManager->ActivateLights(Main);


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

		//glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(myCamera.getMatrix()));
		//glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//glUniform3f(viewPosID, myCamera.getPosition().x, myCamera.getPosition().y, myCamera.getPosition().z);
		/*glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(myCamera.getMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);*/


		renderingManager->RenderAll(myCamera, lightingManager);


		glfwSwapBuffers(window.getWindow());

}