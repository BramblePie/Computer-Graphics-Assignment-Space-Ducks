#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "BaseEntity.h"
#include "Camera.h"

class Scene
{
public:
	Camera camera;

	Scene(GLFWwindow* window) : camera(window)
	{
	}

	// Add an entity to the scene, and return the added entity
	template<class EntityType>
	EntityType& AddEntity(EntityType* entity)
	{
		// Add entity to the map of entities sorted by shader, polymorph to a BaseEntity, and echo added entity back
		return *static_cast<EntityType*>(entities[entity->GetMaterial()->shader].emplace_back(entity).get());
	}

	// Renderloop of scene to be called each frame
	void RenderLoop(const float delta);

private:
	using ShaderEntityMap = std::unordered_map<GLuint, std::vector<std::unique_ptr<BaseEntity>>>;
	ShaderEntityMap entities;
};