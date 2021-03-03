#include "StarEntity.h"

StarMaterial::StarMaterial()
{
	InitShaderProgram(R"(Shaders\starshader.vert)", R"(Shaders\starshader.frag)");
}

void StarMaterial::bind() const
{
}

StarEntity::StarEntity(const glm::vec3& position)
	: BaseEntity(R"(resources\sphere.obj)", init_material()), material(default_mat)
{
	this->position = position;
	this->scale = glm::vec3(.6f, .8f, .6f);
}

const BaseMaterial* StarEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial& StarEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<StarMaterial>();

	return *default_mat.get();
}