#pragma once

#include "BaseEntity.h"

struct DuckMaterial : public BaseMaterial, TexturedMaterialExtension
{
	DuckMaterial();

	// Inherited via BaseMaterial
	virtual const char* GetShaderName() const override { return "duck"; }

protected:
	// Inherited via BaseMaterial
	virtual void bind() const override;
};

class DuckEntity : public BaseEntity
{
public:
	DuckEntity(const glm::vec3& position);

	std::shared_ptr<DuckMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

protected:

private:
	static inline std::shared_ptr<DuckMaterial> default_mat;

	// Inherited via BaseEntity
	virtual void draw() override {};
	virtual const BaseMaterial* init_material() override;
};