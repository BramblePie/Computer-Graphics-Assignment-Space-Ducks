#include "PedestalEntity.h"

PedestalMaterial::PedestalMaterial()
{
	diffuse = { R"(resources\pedestal\MarbleDiffTex2K.png)" };
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
	this->roughness = .15f;
}

PedestalEntity::PedestalEntity(const glm::vec3& position)
	: BaseEntity(R"(resources\pedestal\Pedestal.obj)", init_material()), material(default_mat)
{
	this->position = position;
}

const BaseMaterial* PedestalEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial& PedestalEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<PedestalMaterial>();

	return *default_mat.get();
}