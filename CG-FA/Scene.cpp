#include "Scene.h"

void Scene::RenderLoop(const float delta)
{
	// Draw all entities per shader
	for (auto& shader_slot : entities)
	{
		// Ativate shader
		glUseProgram(shader_slot.first);
		// Set view and projection matrices
		// glGetUniformLocation(shader_slot.first, "u_view") = 0
		glUniformMatrix4fv(0, 1, GL_FALSE, &camera.GetView()[0][0]);
		// glGetUniformLocation(shader_slot.first, "u_projection") = 1
		glUniformMatrix4fv(1, 1, GL_FALSE, &camera.GetProjection()[0][0]);
		// Set "camera_pos"
		glUniform3fv(2, 1, &camera.position[0]);
		// Draw each entity belonging to this shader
		for (auto& entity : shader_slot.second)
		{
			if (entity->Animate)
				entity->Animate(delta);
			entity->Draw();
		}
	}
}