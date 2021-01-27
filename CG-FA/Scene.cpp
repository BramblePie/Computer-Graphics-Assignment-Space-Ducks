#include "Scene.h"

void Scene::RenderLoop(const float delta)
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