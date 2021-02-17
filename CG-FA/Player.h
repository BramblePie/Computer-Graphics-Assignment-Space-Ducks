#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Keybinding.h"
#include "Constants.h"

class Player : IKeyObserver, ICursorObserver, IWindowObserver
{
public:
	float MovementSpeed = 2.0f;
	float LookSensitivity = 6.0f;

	Player() = default;

private:
	struct Orientation
	{
		glm::vec3 position{};
		glm::quat rotation = glm::identity<glm::quat>();

		constexpr glm::vec3 Front()	const { return rotation * WORLD::FRONT; }
		constexpr glm::vec3 Back()	const { return rotation * WORLD::BACK; }
		constexpr glm::vec3 Up()	const { return rotation * WORLD::UP; }
		constexpr glm::vec3 Down()	const { return rotation * WORLD::DOWN; }
		constexpr glm::vec3 Left()	const { return rotation * WORLD::LEFT; }
		constexpr glm::vec3 Right()	const { return rotation * WORLD::RIGHT; }
	} head, body;

	glm::vec3 displacement{};

	float fov = glm::radians(60.0f);
	float window_width = INITIAL::WINDOW_WIDTH;
	float window_height = INITIAL::WINDOW_HEIGHT;

public:
	constexpr glm::vec3 Front()	const { return head.rotation * WORLD::FRONT; }
	constexpr glm::vec3 Back()	const { return head.rotation * WORLD::BACK; }
	constexpr glm::vec3 Up()	const { return head.rotation * WORLD::UP; }
	constexpr glm::vec3 Down()	const { return head.rotation * WORLD::DOWN; }
	constexpr glm::vec3 Left()	const { return head.rotation * WORLD::LEFT; }
	constexpr glm::vec3 Right()	const { return head.rotation * WORLD::RIGHT; }

	constexpr glm::vec3 GetPosition() const { return head.position; }

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(head.position, head.position + head.Front(), head.Up());
	}

	inline glm::mat4 GetProjection() const
	{
		return glm::perspective(fov, window_width / window_height, 0.01f, 100.0f);
	}

	constexpr float GetFOV() const
	{
		return glm::degrees(fov);
	}

	constexpr void SetFOV(const float degrees)
	{
		fov = glm::radians(degrees);
	}

	void Update(const float delta);

	// Inherited via IKeyObserver
	virtual void OnKeyEvent(const int key) override;

	// Inherited via ICursorObserver
	virtual void OnCursorMovement(const float dx, const float dy) override;

	// Inherited via IWindowObserver
	virtual void OnWindowResize(const float width, const float height) override;
};