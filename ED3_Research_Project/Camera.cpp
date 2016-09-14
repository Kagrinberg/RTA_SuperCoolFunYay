#include "Camera.h"
#include "Utilities.h"
#include "GLError.h"
#include "Registry.h"

Camera::Camera() {
	ViewMatrix = glm::mat4(1.0f);
	camera_pos_ = glm::vec3(-5,2,0);

}


glm::mat4 Camera::getMatrix()
{
	return ViewMatrix;
}


// move directions - (forward/backward/0, left/right/0, up/down/0). each direction could be 1, -1 or 0
// rotations - (horizontal_amount, vertical_amount)
void Camera::move(glm::vec3 directions, glm::vec2 rotations, float frametime)
{

	glm::quat pitch = glm::quat(glm::vec3(-rotations.y, 0, 0.f));
	glm::quat yaw = glm::quat(glm::vec3(0, -rotations.x, 0.f));

	camera_orientation_ = glm::normalize(yaw * camera_orientation_ * pitch);

	glm::vec3 camera_roll_direction = camera_orientation_ * glm::vec3(0, 0, -1);
	glm::vec3 camera_pitch_direction = camera_orientation_ * glm::vec3(-1, 0, 0);

	// forward/backward move - all axes could be affected
	camera_pos_ += directions[0] * camera_roll_direction * frametime * camera_speed_;
	// left and right strafe - only xz could be affected    
	camera_pos_ += directions[1] * camera_pitch_direction * frametime * camera_speed_;
	// up and down flying - only y-axis could be affected
	camera_pos_.y += directions[2] * frametime * camera_speed_;

	ViewMatrix = glm::lookAt(camera_pos_, camera_pos_ + camera_roll_direction,
		glm::cross(camera_roll_direction, camera_pitch_direction));


}

void Camera::reset()
{
	camera_orientation_ = glm::quat(glm::vec3(0, 0, 0));
	camera_pos_ = glm::vec3(0, 0, 0);
	ViewMatrix = glm::mat4(1.0);
}

void Camera::update(float frametime)
{
	POINT mouseCur;

	GetCursorPos(&mouseCur);

	glm::vec2 rotation = { 0,0 };
	glm::vec3 directions = { 0,0,0 };
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		rotation.x = (mouseCur.x - mousePrev.x) * 0.03f;
		rotation.y = (mouseCur.y - mousePrev.y) * 0.03f;

	}
	else
	{
		mousePrev.x = 0;
		mousePrev.y = 0;
		rotation.x = 0;
		rotation.y = 0;
	}

	mousePrev = mouseCur;

	if (GetAsyncKeyState('D')) directions.y = -1;
	if (GetAsyncKeyState('A')) directions.y = 1;
	if (GetAsyncKeyState('W')) directions.x = 1;
	if (GetAsyncKeyState('S')) directions.x = -1;
	if (GetAsyncKeyState('E')) directions.z = 1;
	if (GetAsyncKeyState('Q')) directions.z = -1;


	if (GetKeyState('R') & 0x80)
	{
		reset();

	}
	else
	{
		move(directions, rotation, frametime);
	}
}
