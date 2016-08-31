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
//FBX
//Import fbx
//Animate it.

RenderingManager * renderingManager;
unsigned int program;

#pragma region GLOBAL_VARIABLES
unsigned int screenWidth = 1024;
unsigned int screenHeight = 720;

glm::vec4 light = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

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

float theta;
float scaleAmount;
#pragma endregion GLOBAL_VARIABLES

#pragma region FUNCTION_PROTOTYPES
bool LoadTGA(Texture *, char *);	
GLenum Initialize(int argc, char** argv);
void Reshape(int width, int height);
void Render();
void InitializeMatrices();
void InitializeRegistry();
void InitializeSettings();
#pragma endregion FUNCTION_PROTOTYPES

int main(int argc, char** argv){

	//Initialize Glut and setup window
	if(Initialize(argc, argv) != GLEW_OK){
		fprintf(stderr, "Could Not Initialize Glew.");
		return 1;
	}

	//Print version of openGL
	printf("%s\n", glGetString(GL_VERSION));

	//Allocate all of the managers
	ResourceManager * resourceManager = ResourceManager::getInstance();
	EntityManager * entityManager = new EntityManager();
	ShaderManager * shaderManager = new ShaderManager();
	renderingManager = new RenderingManager();

	//Add delegates between managers
	resourceManager->setEntityManager(entityManager);
	entityManager->setRenderingManager(renderingManager);

	//Load Level
	resourceManager->LoadLevel("Level_1");

	//Create Shader Program and set main program.
	shaderManager->CreateProgram("Main", "Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	program = shaderManager->GetShader("Main");

	perspectiveMatrixID = glGetUniformLocation(program, "mP");
	viewMatrixID = glGetUniformLocation(program, "mV");
	modelMatrixID = glGetUniformLocation(program, "mM");
	lightID = glGetUniformLocation(program, "vLight");

	glutMainLoop();

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
}

void InitializeMatrices(){

	theta = 0.0f;
	scaleAmount = 1.0f;

	rotXMatrix = glm::mat4(1.0f);
	rotYMatrix = glm::mat4(1.0f);
	rotZMatrix = glm::mat4(1.0f);

	transMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	tempMatrix1 = glm::mat4(1.0f);

	transMatrix = glm::translate(transMatrix, glm::vec3(0.0f, -0.0f, -1.2f));

	M = glm::mat4(1.0f);
	myCamera.setMatrix(glm::mat4(1.0f));
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glUseProgram(program);

	theta+= 0.00015f;
	scaleAmount= abs(sin(theta));

	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleAmount, scaleAmount,scaleAmount));
	rotYMatrix = glm::rotate(glm::mat4x4(1.0f), theta, glm::vec3(0.5f, 0.0f, 0.0f));
	
	tempMatrix1 = rotYMatrix * scaleMatrix;
	M = transMatrix * tempMatrix1;

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(myCamera.getMatrix()));
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, glm::value_ptr(P));
	glUniform4fv(lightID, 1, glm::value_ptr(light));

	renderingManager->RenderAll();

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();

}

