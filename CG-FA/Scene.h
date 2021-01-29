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
		camera.position.z = .8f;
		camera.position.y = .4f;
		camera.orientation = glm::rotate(camera.orientation, glm::radians(180.0f), WORLD::UP);
	}

	// Add an entity to the scene, and return the added entity
	template<class EntityType>
	EntityType& AddEntity(EntityType* entity)
	{
		// Add entity to the map of entities sorted by shader, polymorph to a BaseEntity, and echo added entity back
		return *static_cast<EntityType*>(entities[entity->GetMaterial()->shader].emplace_back(entity).get());
	}

	void RenderLoop(const float delta);

private:
	using ShaderEntityMap = std::unordered_map<GLuint, std::vector<std::unique_ptr<BaseEntity>>>;
	ShaderEntityMap entities;
};