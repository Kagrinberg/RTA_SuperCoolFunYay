#include "GameWindow.h"

#include <cstdio>

#include "rlutil.h"

#include "GLError.h"

GameWindow::GameWindow() {

	m_screenWidth = 1024;
	m_screenHeight = 720;

}

GameWindow::GameWindow(unsigned int p_screenWidth, unsigned int p_screenHeight) {

	m_screenWidth = p_screenWidth;
	m_screenHeight = p_screenHeight;

}

GameWindow::~GameWindow() {

	glfwTerminate();

}

void GameWindow::Initalize(int argc, char** argv) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "Awakend", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;
	check_gl_error();

	glewInit();
	check_gl_error();

	glEnable(GL_CULL_FACE);
	check_gl_error();

	glCullFace(GL_BACK);
	check_gl_error();

	glEnable(GL_DEPTH_TEST);
	check_gl_error();

	glEnable(GL_BLEND);
	check_gl_error();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	check_gl_error();

	glEnable(GL_MULTISAMPLE);
	check_gl_error();

	glViewport(0, 0, m_screenWidth, m_screenHeight);
	check_gl_error();

	rlutil::setColor(rlutil::LIGHTRED);
	std::cout << "[OpenGL Version] ";
	rlutil::setColor(rlutil::WHITE);
	printf("%s\n", glGetString(GL_VERSION));

}


bool GameWindow::isRunning() {

	return !glfwWindowShouldClose(m_window);
}