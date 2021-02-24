#include "Scene.h"

#include <cstdio>

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

int Scene::GetLightLocation(const GLuint shader, const char* name) const
{
	auto p = std::pair(shader, std::string(name));
	auto itr = light_cache.find(p);
	int i;
	if (itr == std::end(light_cache))
	{
		i = glGetUniformLocation(shader, name);
		light_cache[p] = i;
		if (i < 0)
			printf("[WARNING] Light attribute %s not found in shader %i\n", name, shader);
	}
	else
		i = itr->second;

	return i;
}

void Scene::setLights(const GLuint shader)
{
	if (lights.size() == 0)
		return;

	auto count = lights.size();
	if (count > 8)
	{
		printf("[WARNING] Too many lights in scene only 8 are used\n");
		count = 8;
	}

	char var[26];
	for (size_t i = 0; i < count; i++)
	{
		std::snprintf(var, sizeof(var), "lights[%.1u].color", i);
		glUniform3fv(GetLightLocation(shader, var), 1, &lights[i].color[0]);
		std::snprintf(var, sizeof(var), "lights[%.1u].position", i);
		glUniform3fv(GetLightLocation(shader, var), 1, &lights[i].position[0]);
	}
}