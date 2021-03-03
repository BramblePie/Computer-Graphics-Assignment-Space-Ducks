#pragma once

#include "BaseEntity.h"

struct StarMaterial : public BaseMaterial
{
	StarMaterial();

	// Inherited via BaseMaterial
	virtual inline const char* GetShaderName() const override { return "simple"; }
	virtual void bind() const override;
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
	virtual void draw() override {};
	virtual const BaseMaterial& init_material() override;
};