#pragma once

#include <glm/glm.hpp>

namespace WORLD
{
	// Direction of world up
	static constexpr const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	// Direction of world down
	static constexpr const glm::vec3 DOWN = -WORLD::UP;

	// Direction of world front
	static constexpr const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
	// Direction of world back
	static constexpr const glm::vec3 BACK = -WORLD::FRONT;

	// Direction of world left
	static constexpr const glm::vec3 LEFT = glm::vec3(1.0f, 0.0f, 0.0f);
	// Direction of world right
	static constexpr const glm::vec3 RIGHT = -WORLD::LEFT;
}

namespace CONSTS
{
	template<class T>
	static constexpr const T SQRT2 = 1.4142135623730950488016887242;

	template<class T>
	static constexpr const T INVSQRT2 = 0.7071067811865475244008443621;
}

namespace INITIAL
{
	// Initial window width
	static constexpr const int WINDOW_WIDTH = 1200;
	// Initial window height
	static constexpr const int WINDOW_HEIGHT = 800;
}