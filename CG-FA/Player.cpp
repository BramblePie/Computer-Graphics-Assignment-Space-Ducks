#include "Player.h"

#include <iostream>

void Player::Update(const float delta)
{
	if (glm::dot(displacement, displacement) > 0.01f)
		head += glm::normalize(displacement) * delta * MovementSpeed;
	displacement = {};

	printf("%f, %f, %f\n", head.x, head.y, head.z);
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

void Player::OnWindowResize(const float width, const float height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}