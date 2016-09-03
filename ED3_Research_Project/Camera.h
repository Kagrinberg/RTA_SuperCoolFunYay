#pragma once

#include <windows.h>									// Standard windows header
#include <stdio.h>		
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
	glm::mat4 ViewMatrix;
	glm::vec3 camera_pos_;
	glm::quat camera_orientation_;
	const float camera_speed_ = 0.5;

public:
	Camera();

	glm::mat4 getMatrix();

	void setMatrix(glm::mat4 newMatrix);

	void move(glm::vec3 directions, glm::vec2 rotations, float frametime);

	void reset();

	void update(float frametime);

};