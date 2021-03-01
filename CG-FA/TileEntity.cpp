#include "TileEntity.h"

#include "Constants.h"

TileMaterial::TileMaterial()
{
	diffuse = { R"(resources\tile\tile_diffuse.png)" };
	normal = { R"(resources\tile\tile_normal.png)", false };
	InitShaderProgram(R"(Shaders\default.vert)", R"(Shaders\default.frag)");
}

TileEntity::TileEntity(const glm::vec3& position)
	: BaseEntity(R"(resources\tile\tile.obj)", init_material()), material(default_mat)
{
	this->position = position;
	const size_t r = reinterpret_cast<size_t>(this) / 8 % 4;
	switch (r)
	{
	case 1:
		this->orientation = glm::quat(CONSTS::INVSQRT_2<float>, .0f, CONSTS::INVSQRT_2<float>, .0f);
		break;
	case 2:
		this->orientation = glm::quat(.0f, .0f, 1.0f, .0f);
		break;
	case 3:
		this->orientation = glm::quat(CONSTS::INVSQRT_2<float>, .0f, -CONSTS::INVSQRT_2<float>, .0f);
		break;
	default:
		// Default orientation
		break;
	}
}

const BaseMaterial* TileEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial& TileEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<TileMaterial>();

	return *default_mat.get();
}