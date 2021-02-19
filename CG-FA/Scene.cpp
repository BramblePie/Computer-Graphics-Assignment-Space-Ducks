#include "Scene.h"

void Scene::RenderLoop(const float delta)
{
	// Update player for next frame
	player.Update(delta);

	// Draw all entities per shader
	for (auto& shader_slot : entities)
	{
		// Ativate shader
		glUseProgram(shader_slot.first);
		// Set view and projection matrices
		// glGetUniformLocation(shader_slot.first, "u_view") = 0
		glUniformMatrix4fv(0, 1, GL_FALSE, &player.GetView()[0][0]);
		// glGetUniformLocation(shader_slot.first, "u_projection") = 1
		glUniformMatrix4fv(1, 1, GL_FALSE, &player.GetProjection()[0][0]);
		// Set "camera_pos"
		glUniform3fv(2, 1, &player.GetPosition()[0]);

		// Set lights for each shader
		setLights(shader_slot.first);

		// Draw each entity belonging to this shader
		for (auto& entity : shader_slot.second)
		{
			if (entity->Animate)
				entity->Animate(delta);
			entity->Draw();
		}
	}
}

void Scene::setLights(const GLuint shader)
{
	if (lights.size() == 0)
		return;
	// Get location of lights uniform
	GLint loc = glGetUniformLocation(shader, "lights[0].color");	// TODO Very fucking expensive
	if (loc < 0)
		return;

	auto lcount = lights.size();
	if (lcount > 8)
	{
		printf("[WARNING] Too many lights in scene only 8 are used\n");
		lcount = 8;
	}

	for (size_t i = 0; i < lcount; i++)
	{
		glUniform3fv(loc + i, 1, &lights[i].color[0]);
		glUniform3fv(loc + 1 + i, 1, &lights[i].position[0]);
	}
}