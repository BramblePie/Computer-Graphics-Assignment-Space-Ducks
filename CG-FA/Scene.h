#pragma once

#include <vector>
#include <memory>

#include "BasicEntity.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	std::vector<BasicEntity> entities;

	void RenderLoop(const float delta)
	{
		for (auto& e : entities)
			e.Draw();
	}

private:
};