#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "BaseEntity.h"
#include "Camera.h"

class Scene
{
public:
	Scene(GLFWwindow* window) : camera(window)
	{
		camera.position.z = 1.0f;
		camera.position.y = .5f;
		camera.orientation = glm::rotate(camera.orientation, glm::radians(180.0f), WORLD::UP);
	}

	using ShaderEntityMap = std::unordered_map<GLuint, std::vector<std::unique_ptr<BaseEntity>>>;
	ShaderEntityMap entities;

	Camera camera;

	// Add an entity to the scene, and echo the added entity
	template<class EntityType>
	EntityType* AddEntity(EntityType* entity)
	{
		return static_cast<EntityType*>(entities[entity->GetMaterial()->shader].emplace_back(entity).get());
	}

	void RenderLoop(const float delta)
	{
		// Draw all entities per shader
		for (auto& shader_slot : entities)
		{
			// Ativate shader
			glUseProgram(shader_slot.first);
			// Set view and projection matrices
			glUniformMatrix4fv(glGetUniformLocation(shader_slot.first, "u_view"),
							   1, GL_FALSE, &camera.GetView()[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_slot.first, "u_projection"),
							   1, GL_FALSE, &camera.GetProjection()[0][0]);
			// Draw each entity belonging to this shader
			for (auto& entity : shader_slot.second)
				entity->Draw();
		}
	}

private:
};