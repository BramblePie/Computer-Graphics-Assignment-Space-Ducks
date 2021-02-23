#include "PedestalEntity.h"

PedestalMaterial::PedestalMaterial()
{
	diffuse = { R"(resources\pedestal\MarbleDiffTex2K.png)" };
	InitShaderProgram(R"(Shaders\BasicVertex.vert)", R"(Shaders\PBRFragment.frag)");
	this->roughness = .16f;
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

const BaseMaterial* PedestalEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<PedestalMaterial>();
	material = default_mat;

	return material.get();
}