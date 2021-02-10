#include "DuckEntity.h"

#include <utility>

DuckMaterial::DuckMaterial()
{
	diffuse = { R"(C:\Users\bramp\Desktop\duck\duck_diffuse.png)" };
	InitShaderProgram(R"(Shaders\BasicVertex.vert)", R"(Shaders\PBRFragment.frag)");

	struct Light
	{
		glm::vec3 position;
		glm::vec3 color;
	};
	// TODO Lighting
	glUseProgram(shader);
	Light l = { glm::vec3(1.0f, 2.0f, .4f), glm::vec3(1.0f) };
	glUniform3fv(glGetUniformLocation(shader, "lights[0].position"), 1, &l.position[0]);
	glUniform3fv(glGetUniformLocation(shader, "lights[0].color"), 1, &l.color[0]);
}

void DuckMaterial::bind() const
{
	SetUniform(metallic, MAT_METALLIC);
	SetUniform(roughness, MAT_ROUGHNESS);
	SetUniform(diffuse.value(), TEX_DIFFUSE);
}

DuckEntity::DuckEntity(const glm::vec3& position)
	: BaseEntity(R"(C:\Users\bramp\Desktop\duck\duck.obj)", init_material()), material(default_mat)
{
	this->position = position;
}

const BaseMaterial* DuckEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial* DuckEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<DuckMaterial>();
	material = default_mat;

	return material.get();
}