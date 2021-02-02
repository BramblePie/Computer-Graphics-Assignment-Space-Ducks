#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Constants.h"

class Camera
{
public:

	glm::vec3 position{};
	glm::quat orientation = glm::identity<glm::quat>();

	Camera(GLFWwindow* window, float fov_degree = 60.0f) : window(window), fov(glm::radians(fov_degree)) {};

	glm::mat4 GetView() const;

	glm::mat4 GetProjection() const;

	// Gets field of view in degrees
	float GetFOV() const;

	// Sets field of view in degrees
	void SetFOV(float degrees);

	// Make the camera point at a point in space
	void PointAt(glm::vec3 focus)
	{
		orientation = glm::rotation(Front(), glm::normalize(focus - position));
	}

	glm::vec3 Front() const;

	glm::vec3 Up() const;

	glm::vec3 Left() const;

private:
	GLFWwindow* window;
	mutable int _width = 0, _heigth = 0;

	// Field of view in radians
	float fov;
};