#pragma once

#include <windows.h>									// Standard windows header
#include <stdio.h>		
#include "glm/gtc/type_ptr.hpp"

class Camera
{
	glm::mat4 ViewMatrix;

public:
	Camera();

	glm::mat4 getMatrix();

	void setMatrix(glm::mat4 newMatrix);

};