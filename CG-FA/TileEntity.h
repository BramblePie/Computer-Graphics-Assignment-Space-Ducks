#pragma once

#include "BaseEntity.h"

struct TileMaterial : public TexturedMaterial
{
	TileMaterial();

private:
};

class TileEntity : public BaseEntity
{
public:
	TileEntity(const glm::vec3& position);

	std::shared_ptr<TileMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

private:
	static inline std::shared_ptr<TileMaterial> default_mat;

	// Inherited via BaseEntity
	virtual void draw() override {};
	virtual const BaseMaterial& init_material() override;
};
