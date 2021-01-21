#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "BasicEntity.h"
#include "Camera.h"

class Scene
{
public:
	Scene(GLFWwindow* window) : camera(window)
	{
		camera.position.z = -1.0f;
		camera.position.x = -1.0f;
		camera.orientation = glm::rotate(camera.orientation, glm::radians(45.0f), WORLD::UP);
	}

	~Scene() = default;

	std::vector<BasicEntity> entitiesold;

	using ShaderEntityMap = std::unordered_map<GLuint, std::vector<BaseEntity*>>;
	ShaderEntityMap entities;

	Camera camera;

	void RenderLoop(const float delta)
	{
		// Draw all entities per shader
		for (auto& shader_slot : entities)
		{
			// Ativate shader
			glUseProgram(shader_slot.first);
			// Set view and projection matrices
			glUniformMatrix4fv(glGetUniformLocation(shader_slot.first, "view"),
							   1, GL_FALSE, &camera.GetView()[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_slot.first, "projection"),
							   1, GL_FALSE, &camera.GetProjection()[0][0]);
			// Draw each entity belonging to this shader
			for (BaseEntity* entity : shader_slot.second)
				entity->Draw();
		}
	}

private:
};