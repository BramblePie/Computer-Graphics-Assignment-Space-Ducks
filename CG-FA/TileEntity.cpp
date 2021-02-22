#include "TileEntity.h"

TileMaterial::TileMaterial()
{
	diffuse = { R"(resources\tile\tile_diffuse.png)" };
	normal = { R"(resources\tile\tile_normal.png)" };
	InitShaderProgram(R"(Shaders\BasicVertex.vert)", R"(Shaders\PBRFragment.frag)");
}

TileEntity::TileEntity(const glm::vec3& position)
	: BaseEntity(R"(resources\tile\tile.obj)", init_material()), material(default_mat)
{
	this->position = position;
}

const BaseMaterial* TileEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial* TileEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<TileMaterial>();
	material = default_mat;

	return material.get();
}