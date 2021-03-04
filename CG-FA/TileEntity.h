#pragma once

#include "BaseEntity.h"

/*	All entities are mustly the same, only different types
*	DuckEntity.h has more explanation
*/

struct TileMaterial : public TexturedMaterial
{
	TileMaterial();
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
	virtual const BaseMaterial& init_material() override;
};
