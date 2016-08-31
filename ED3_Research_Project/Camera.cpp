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