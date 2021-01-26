#pragma once

#include "BaseEntity.h"

class DuckEntity : public BaseEntity
{
public:
	using DuckMaterial = DebugMaterial;

	DuckEntity();

	std::shared_ptr<DuckMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

private:
	static inline std::shared_ptr<DuckMaterial> default_mat;

	const BaseMaterial* init_material();

	// Inherited via BaseEntity
	virtual void draw() override {};
};