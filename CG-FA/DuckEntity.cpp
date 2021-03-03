#include "DuckEntity.h"

#include <utility>

DuckMaterial::DuckMaterial()
{
	diffuse = { R"(resources\duck\duck_diffuse.png)" };
	gloss = { R"(resources\duck\duck_gloss.png)", false };
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
}

DuckMaterial::DuckMaterial(const glm::vec3& color, const float metallic)
{
	this->color = color;
	this->metallic = metallic;
	gloss = { R"(resources\duck\duck_gloss.png)", false };
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
}

DuckMaterial::DuckMaterial(const glm::vec3& color, const float roughness, const float metallic)
{
	this->color = color;
	this->metallic = metallic;
	this->roughness = roughness;
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
}

DuckEntity::DuckEntity(const glm::vec3& position, const glm::quat& orientation)
	: BaseEntity(R"(resources\duck\duck.obj)", init_material()), material(default_mat)
{
	this->position = position;
	this->orientation = orientation;
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