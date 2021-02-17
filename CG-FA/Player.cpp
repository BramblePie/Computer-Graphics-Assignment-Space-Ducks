#include "Player.h"

#include <iostream>

//#include <glm/gtx/quaternion.hpp>

void Player::Update(const float delta)
{
	if (glm::dot(displacement, displacement) > 0.01f)
		head.position += glm::normalize(displacement) * delta * MovementSpeed;
	displacement = {};
}

// Inherited via IKeyObserver
void Player::OnKeyEvent(const int key)
{
	switch (key)
	{
	case GLFW_KEY_W:
		displacement += Front();
		break;
	case GLFW_KEY_A:
		displacement += Left();
		break;
	case GLFW_KEY_S:
		displacement += Back();
		break;
	case GLFW_KEY_D:
		displacement += Right();
		break;
	case GLFW_KEY_LEFT_CONTROL:
		displacement += Down();
		break;
	case GLFW_KEY_SPACE:
		displacement += Up();
		break;
	default:
		break;
	}
}

// Inherited via ICursorObserver
void Player::OnCursorMovement(const float dx, const float dy)
{
	// Check if head is upside down, in that case flip yaw direction
	const float isUpright = glm::dot(body.Up(), head.Up());
	const float flip = isUpright < .0f ? -1.0f : 1.0f;
	// Scale down sensitivity
	const float s = -0.0001f;

	// Get yaw rotation around up axis of body
	const auto yaw = glm::rotate(glm::identity<glm::quat>(), flip * LookSensitivity * s * dx, body.Up());
	// Get pitch rotation around left axis of body
	const auto pitch = glm::rotate(glm::identity<glm::quat>(), LookSensitivity * s * dy, body.Left());
	// Apply rotations to body and head
	body.rotation = yaw * body.rotation;
	head.rotation = yaw * pitch * head.rotation;
}

// Inherited via IWindowObserver
void Player::OnWindowResize(const float width, const float height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}