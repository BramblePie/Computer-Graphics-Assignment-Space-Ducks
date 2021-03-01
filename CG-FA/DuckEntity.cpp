#include "DuckEntity.h"

#include <utility>

DuckMaterial::DuckMaterial()
{
	diffuse = { R"(resources\duck\duck_diffuse.png)" };
	gloss = { R"(resources\duck\duck_gloss.png)", false };
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
}

DuckMaterial::DuckMaterial(const bool noTexture)
{
	if (noTexture)
		InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
	else
	{
		diffuse = { R"(resources\duck\duck_diffuse.png)" };
		gloss = { R"(resources\duck\duck_gloss.png)", false };
		InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
	}
}

DuckEntity::DuckEntity(const glm::vec3& position)
	: BaseEntity(R"(resources\duck\duck.obj)", init_material()), material(default_mat)
{
	this->position = position;
}

const BaseMaterial* DuckEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial& DuckEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<DuckMaterial>();

	return *default_mat.get();
}