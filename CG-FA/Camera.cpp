#include "Camera.h"

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + Front(), WORLD::UP);
}

glm::mat4 Camera::GetProjection() const
{
	int width, heigth;
	glfwGetWindowSize(window, &width, &heigth);
	if (width != _width || heigth != _heigth)
	{	// TODO: Might need some change, no need to ask for size each frame
		_width = width; _heigth = heigth;
		glViewport(0, 0, width, heigth);
	}
	return glm::perspective(fov, static_cast<float>(width) / static_cast<float>(heigth), 0.01f, 100.0f);
}

float Camera::GetFOV() const
{
	return glm::degrees(fov);
}

void Camera::SetFOV(float degrees)
{
	fov = glm::radians(degrees);
}

glm::vec3 Camera::Front() const
{
	return orientation * WORLD::FRONT;
}

glm::vec3 Camera::Up() const
{
	return orientation * WORLD::UP;
}

glm::vec3 Camera::Left() const
{
	return orientation * WORLD::LEFT;
}