#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Constants.h"

class Camera
{
public:

	glm::vec3 position{};
	glm::quat orientation = glm::identity<glm::quat>();

	Camera(GLFWwindow* window, float fov_degree = 60.0f) : window(window), fov(glm::radians(fov_degree)) {};

	glm::mat4 GetView() const
	{
		return glm::lookAt(position, position + Front(), WORLD::UP);
	}

	glm::mat4 GetProjection() const
	{
		int width, heigth;
		glfwGetWindowSize(window, &width, &heigth);
		return glm::perspective(fov, static_cast<float>(width) / static_cast<float>(heigth), 0.01f, 100.0f);
	}

	// Gets field of view in degrees
	float GetFOV() const
	{
		return glm::degrees(fov);
	}

	// Sets field of view in degrees
	void SetFOV(float degrees)
	{
		fov = glm::radians(degrees);
	}

	glm::vec3 Front() const
	{
		return orientation * WORLD::FRONT;
	}

	glm::vec3 Up() const
	{
		return orientation * WORLD::UP;
	}

	glm::vec3 Left() const
	{
		return orientation * WORLD::LEFT;
	}

private:
	GLFWwindow* window;

	// Field of view in radians
	float fov;
};