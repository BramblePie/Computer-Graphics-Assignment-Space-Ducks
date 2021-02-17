#pragma once

#include <glm/glm.hpp>

namespace WORLD
{
	static constexpr const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr const glm::vec3 DOWN = -WORLD::UP;

	static constexpr const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
	static constexpr const glm::vec3 BACK = -WORLD::FRONT;

	static constexpr const glm::vec3 LEFT = glm::vec3(1.0f, 0.0f, 0.0f);
	static constexpr const glm::vec3 RIGHT = -WORLD::LEFT;
}