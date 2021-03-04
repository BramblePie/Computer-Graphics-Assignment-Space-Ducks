#pragma once

#include "BaseEntity.h"

/*	All entities are mustly the same, only different types
*	DuckEntity.h has more explanation
*/

struct PedestalMaterial : public TexturedMaterial
{
	PedestalMaterial();
};

class PedestalEntity : public BaseEntity
{
public:
	PedestalEntity(const glm::vec3& position);

	std::shared_ptr<PedestalMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

private:
	static inline std::shared_ptr<PedestalMaterial> default_mat;

	// Inherited via BaseEntity
	virtual const BaseMaterial& init_material() override;
};
