#include "Player.h"

#include <iostream>

//#include <glm/gtx/quaternion.hpp>

void Player::Update(const float delta)
{
	if (glm::dot(displacement, displacement) > 0.01f)
	{
		const auto move = glm::normalize(displacement) * delta * MovementSpeed;
		if (!isFlying)
			body.position += move;
		head.position += move;
	}
	displacement = {};
}

// Inherited via IKeyObserver
void Player::OnKeyEvent(const int key)
{
	if (isFlying)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			displacement += head.Front();
			break;
		case GLFW_KEY_A:
			displacement += head.Left();
			break;
		case GLFW_KEY_S:
			displacement += head.Back();
			break;
		case GLFW_KEY_D:
			displacement += head.Right();
			break;
		case GLFW_KEY_LEFT_CONTROL:
			displacement += head.Down();
			break;
		case GLFW_KEY_SPACE:
			displacement += head.Up();
			break;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case GLFW_KEY_W:
			displacement += body.Front();
			break;
		case GLFW_KEY_A:
			displacement += body.Left();
			break;
		case GLFW_KEY_S:
			displacement += body.Back();
			break;
		case GLFW_KEY_D:
			displacement += body.Right();
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
	window_width = width;
	window_height = height;
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}