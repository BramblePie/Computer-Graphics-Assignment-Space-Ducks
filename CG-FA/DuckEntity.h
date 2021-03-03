#pragma once

#include "BaseEntity.h"

struct DuckMaterial : public TexturedMaterial
{
	// Create a duck material with both a diffuse map and a gloss map
	DuckMaterial();
	// Create a duck material of a specified color and metalness, and with the gloss map
	DuckMaterial(const glm::vec3& color, const float metallic = .0f);
	// Create a duck material of a specified color, roughness and metalness
	DuckMaterial(const glm::vec3& color, const float roughness, const float metallic);

protected:
};

class DuckEntity : public BaseEntity
{
public:
	DuckEntity(const glm::vec3& position, const glm::quat& orientation);

	std::shared_ptr<DuckMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

protected:

private:
	static inline std::shared_ptr<DuckMaterial> default_mat;

	// Inherited via BaseEntity
	virtual void draw() override {};
	virtual const BaseMaterial& init_material() override;
};