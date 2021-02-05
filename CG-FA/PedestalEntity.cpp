#include "PedestalEntity.h"

PedestalMaterial::PedestalMaterial()
{
	diffuse = { R"(C:\Users\bramp\Desktop\Pedestal\MarbleDiffTex2K.png)" };
	InitShaderProgram(R"(Shaders\BasicVertex.vert)", R"(Shaders\BasicFragment.frag)");
}

void PedestalMaterial::bind() const
{
	SetUniform(diffuse, TEX_DIFFUSE);
}

PedestalEntity::PedestalEntity(const glm::vec3& position)
	: BaseEntity(R"(C:\Users\bramp\Desktop\Pedestal\Pedestal.obj)", init_material()), material(default_mat)
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