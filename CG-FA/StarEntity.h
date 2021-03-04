#pragma once

#include "BaseEntity.h"

/*	All entities are mustly the same, only different types
*	DuckEntity.h has more explanation
*/

struct StarMaterial : public BaseMaterial
{
	// A star has no texture and its material is an extention of BaseMaterial

	StarMaterial();

	// Inherited via BaseMaterial
	virtual inline const char* GetShaderName() const override { return "star"; }
	virtual void bind() const override {};
};

class StarEntity : public BaseEntity
{
public:
	StarEntity(const glm::vec3& position);

	std::shared_ptr<StarMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

private:
	static inline std::shared_ptr<StarMaterial> default_mat;

	// Inherited via BaseEntity
	virtual const BaseMaterial& init_material() override;
};