#include "Utilities.h"
#include "glm/gtc/type_ptr.hpp"
#include "ResourceManager.h"
#include "RenderingManager.h"
#include "EntityManager.h"
#include "Renderable.h"
#include "Registry.h"
#include "Transform.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "XTime.h"
//FBX
//Import fbx
//Animate it.

ResourceManager * resourceManager;
RenderingManager * renderingManager;
EntityManager * entityManager;
ShaderManager * shaderManager;
LightingManager * lightingManager;

unsigned int program;

#pragma region GLOBAL_VARIABLES
unsigned int screenWidth = 1024;
unsigned int screenHeight = 720;

unsigned int perspectiveMatrixID, viewMatrixID, modelMatrixID, rotMatrixID, lightID;

glm::mat4 rotXMatrix;
glm::mat4 rotYMatrix;
glm::mat4 rotZMatrix;

glm::mat4 transMatrix;
glm::mat4 scaleMatrix;
glm::mat4 tempMatrix1;

glm::mat4 M;
glm::mat4 P;

Camera myCamera;
XTime timer;

float theta;
float scaleAmount;
#pragma endregion GLOBAL_VARIABLES

#pragma region FUNCTION_PROTOTYPES
GLenum Initialize(int argc, char** argv);
void Reshape(int width, int height);
void Render();
void InitializeMatrices();
void InitializeRegistry();
void CleanUp();
#pragma endregion FUNCTION_PROTOTYPES

int main(int argc, char** argv){

	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetBreakAlloc(-1);

	//Initialize Glut and setup window
	if(Initialize(argc, argv) != GLEW_OK){
		fprintf(stderr, "Could Not Initialize Glew.");
		return 1;
	}

	timer.Restart();

	//Print version of openGL
	printf("%s\n", glGetString(GL_VERSION));

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
	program = shaderManager->GetShader("Main");

	perspectiveMatrixID = glGetUniformLocation(program, "projection");
	viewMatrixID = glGetUniformLocation(program, "view");
	modelMatrixID = glGetUniformLocation(program, "model");

	//Use Program
	glUseProgram(program);

	//Intialize Camera
	myCamera.Initialize();

	//Activate Lights
	lightingManager->ActivateLights();

	//Sort Renderables
	renderingManager->sortByMaterial();

	glutMainLoop();

	CleanUp();

	return 0;
}


//Initialization
GLenum Initialize(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("ED3_RESEARCH_PROJECT");
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);
		
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE); 

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);


	//Intialize Matricies.
	InitializeMatrices();

	//Initalize Registry
	InitializeRegistry();

	//Initalize Glew
	return glewInit();
}

void InitializeRegistry(){
	register_type(Renderable);
	register_type(Transform);
	register_type(Light);
}

void InitializeMatrices(){


	P = glm::mat4(1.0f);

}


//OpenGL
void Reshape(int width, int height){

	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;
	P = glm::perspectiveFov(1.0f/tan(60.0f*3.1415926f/360.0f), (float)screenWidth, (float)screenHeight, 0.001f, 1000.0f);

}

void Render(){

	timer.Signal();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glUseProgram(program);

	myCamera.update(timer.Delta());

	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(myCamera.getMatrix()));
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, glm::value_ptr(P));

	renderingManager->RenderAll();

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();

}

void CleanUp() {
	delete resourceManager;
	delete entityManager;
	delete shaderManager;
	delete renderingManager;
	delete lightingManager;
	delete Registry::getInstance();
}