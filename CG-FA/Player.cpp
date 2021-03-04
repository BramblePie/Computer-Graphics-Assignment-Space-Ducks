#include "Player.h"

#include <iostream>

//#include <glm/gtx/quaternion.hpp>

void Player::Update(const float delta)
{
	// If there is a direction to move to
	if (glm::dot(direction, direction) > 0.01f)
	{
		// Normalize direction and move in that direction at movement speed ( with delta )
		const auto move = glm::normalize(direction) * delta * MovementSpeed;
		// If the player isn;t flying also move the body
		if (!isFlying)
			body.position += move;
		// Always move the head
		head.position += move;
	}
	// Clear direction
	direction = {};
}

// Inherited via IKeyObserver
void Player::OnKeyEvent(const int key)
{
	// Controls if player is flying
	if (isFlying)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			direction += head.Front();
			break;
		case GLFW_KEY_A:
			direction += head.Left();
			break;
		case GLFW_KEY_S:
			direction += head.Back();
			break;
		case GLFW_KEY_D:
			direction += head.Right();
			break;
		case GLFW_KEY_LEFT_CONTROL:
			direction += head.Down();
			break;
		case GLFW_KEY_SPACE:
			direction += head.Up();
			break;
		default:
			break;
		}
	}
	else
	{	// Controls if player is walking
		switch (key)
		{
		case GLFW_KEY_W:
			direction += body.Front();
			break;
		case GLFW_KEY_A:
			direction += body.Left();
			break;
		case GLFW_KEY_S:
			direction += body.Back();
			break;
		case GLFW_KEY_D:
			direction += body.Right();
			break;
		default:
			break;
		}
	}
}

// Inherited via IKeyObserver
void Player::OnKeyPress(const int key)
{
	if (isFlying)
	{
		switch (key)
		{
		case GLFW_KEY_C:
			// Stop flying
			isFlying = false;
			head.position = body.position;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case GLFW_KEY_C:
			isFlying = true;
			break;
		default:
			break;
		}
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
	const auto pitchyaw = glm::rotate(yaw, LookSensitivity * s * dy, body.Left());
	// Apply rotations to body and head
	body.rotation = glm::normalize(yaw * body.rotation);
	head.rotation = glm::normalize(pitchyaw * head.rotation);
}

// Inherited via IWindowObserver
void Player::OnWindowResize(const float width, const float height)
{
	// Change width and height
	window_width = width;
	window_height = height;
	// Update opengl viewport
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}