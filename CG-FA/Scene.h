#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "Entities.h"
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

	~Scene()
	{
		for (auto& s : entities)
			for (BaseEntity* e : s.second)
				delete e;
	}

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