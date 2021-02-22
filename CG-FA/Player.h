#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Keybinding.h"
#include "Constants.h"

class Player : IKeyObserver, ICursorObserver, IWindowObserver
{
public:
	// Movement speed of player
	float MovementSpeed = 1.0f;
	// Look sensitivity for mouse control
	float LookSensitivity = 6.0f;

	Player() = default;

private:
	// Orientations of both head and body separately
	struct Orientation
	{
		glm::vec3 position = glm::vec3(.0f, 1.8f, -1.0f);
		glm::quat rotation = glm::identity<glm::quat>();

		constexpr glm::vec3 Front()	const { return rotation * WORLD::FRONT; }
		constexpr glm::vec3 Back()	const { return rotation * WORLD::BACK; }
		constexpr glm::vec3 Up()	const { return rotation * WORLD::UP; }
		constexpr glm::vec3 Down()	const { return rotation * WORLD::DOWN; }
		constexpr glm::vec3 Left()	const { return rotation * WORLD::LEFT; }
		constexpr glm::vec3 Right()	const { return rotation * WORLD::RIGHT; }
	} head, body;

	// Is the player flying?
	bool isFlying = false;

	// The direction the player is to move to
	glm::vec3 direction{};

	// Field of view in radians
	float fov = glm::radians(60.0f);

	// Width of window
	float window_width = INITIAL::WINDOW_WIDTH;
	// Height of window
	float window_height = INITIAL::WINDOW_HEIGHT;

public:

	// Gets current position of player
	constexpr glm::vec3 GetPosition() const
	{
		return isFlying ? head.position : body.position;
	}

	// Gets view from player
	inline glm::mat4 GetView() const
	{
		return glm::lookAt(GetPosition(), GetPosition() + head.Front(), head.Up());
	}

	// Gets projection
	inline glm::mat4 GetProjection() const
	{
		return glm::perspective(fov, window_width / window_height, 0.01f, 100.0f);
	}

	// Gets field of view of player in degrees
	constexpr float GetFOV() const
	{
		return glm::degrees(fov);
	}

	// Sets field of view of player in degrees
	constexpr void SetFOV(const float degrees)
	{
		fov = glm::radians(degrees);
	}

	// Update player movement
	void Update(const float delta);

	// Inherited via IKeyObserver
	virtual void OnKeyEvent(const int key) override;
	virtual void OnKeyPress(const int key) override;

	// Inherited via ICursorObserver
	virtual void OnCursorMovement(const float dx, const float dy) override;

	// Inherited via IWindowObserver
	virtual void OnWindowResize(const float width, const float height) override;
};