#include "Camera.h"

Camera::Camera() {
}

glm::mat4 Camera::getMatrix()
{
	return ViewMatrix;
}

void Camera::setMatrix(glm::mat4 newMatrix)
{
	ViewMatrix = newMatrix;
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