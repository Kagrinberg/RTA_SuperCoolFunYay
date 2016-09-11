#pragma once

#include <windows.h>
#include <stdio.h>		
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
	glm::mat4 ViewMatrix;
	glm::vec3 camera_pos_;
	glm::quat camera_orientation_;
	const float camera_speed_ = 2.0;
	POINT mousePrev;
	unsigned int viewPosLoc;


public:
	Camera();


	void move(glm::vec3 directions, glm::vec2 rotations, float frametime);
	void update(float frametime);
	void reset();

	glm::mat4 getMatrix();
	glm::vec3 getPosition() { return camera_pos_; };



};