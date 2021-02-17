#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Keybinding.h"
#include "Constants.h"

class Player : IKeyObserver
{
public:
	float MovementSpeed = 2.0f;

	Player() {}

private:
	glm::vec3 head{};
	glm::vec3 body{};
	glm::quat orientation = glm::identity<glm::quat>();
	glm::vec3 displacement{};

	float fov = glm::radians(60.0f);

public:
	constexpr glm::vec3 Front()	const { return orientation * WORLD::FRONT; }
	constexpr glm::vec3 Back()	const { return orientation * WORLD::BACK; }
	constexpr glm::vec3 Up()	const { return orientation * WORLD::UP; }
	constexpr glm::vec3 Down()	const { return orientation * WORLD::DOWN; }
	constexpr glm::vec3 Left()	const { return orientation * WORLD::LEFT; }
	constexpr glm::vec3 Right()	const { return orientation * WORLD::RIGHT; }

	constexpr glm::vec3 GetPosition() const { return head; }

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(head, head + Front(), WORLD::UP);
	}

	inline float GetFOV() const
	{
		return glm::degrees(fov);
	}

	inline void SetFOV(const float degrees)
	{
		fov = glm::radians(degrees);
	}

	void Update(const float delta);

	// Inherited via IKeyObserver
	virtual void OnKeyEvent(const int key) override;
};