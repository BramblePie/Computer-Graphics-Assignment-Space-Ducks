#include "DuckEntity.h"

DuckMaterial::DuckMaterial() : diffuse(R"(C:\Users\bramp\Desktop\duck\duck_diffuse.png)")
{
	InitShaderProgram(R"(Shaders\BasicVertex.shader)", R"(Shaders\BasicFragment.shader)");
}

void DuckMaterial::bind() const
{
	SetUniform(color, MAT_COLOR);
	SetUniform(diffuse.unit, TEX_DIFFUSE);
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