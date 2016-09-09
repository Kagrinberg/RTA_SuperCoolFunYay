#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"


class GameWindow {

private:

	GLFWwindow * m_window;
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;


public:

	GameWindow();
	GameWindow(unsigned int p_screenWidth, unsigned int p_screenHeight);

	~GameWindow();

	void Initalize(int argc, char** argv);
	bool isRunning();

	GLFWwindow * getWindow() { return m_window; };

};

#endif 