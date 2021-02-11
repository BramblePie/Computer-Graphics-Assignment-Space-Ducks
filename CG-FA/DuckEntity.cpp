#include "DuckEntity.h"

#include <utility>

DuckMaterial::DuckMaterial()
{
	diffuse = { R"(resources\duck\duck_diffuse.png)" };
	InitShaderProgram(R"(Shaders\BasicVertex.vert)", R"(Shaders\PBRFragment.frag)");
}

void DuckMaterial::bind() const
{
	SetUniform(metallic, MAT_METALLIC);
	SetUniform(roughness, MAT_ROUGHNESS);
	SetUniform(diffuse.value(), TEX_DIFFUSE);
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

const BaseMaterial* DuckEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<DuckMaterial>();
	material = default_mat;

	return material.get();
}