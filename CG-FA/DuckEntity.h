#pragma once

#include "BaseEntity.h"

// Material type that is used for all ducks
struct DuckMaterial : public TexturedMaterial
{
	// Create a duck material with both a diffuse map and a gloss map
	DuckMaterial();
	// Create a duck material of a specified color and metalness, and with the gloss map
	DuckMaterial(const glm::vec3& color, const float metallic = .0f);
	// Create a duck material of a specified color, roughness and metalness
	DuckMaterial(const glm::vec3& color, const float roughness, const float metallic);
};

class DuckEntity : public BaseEntity
{
public:
	// Create a duck at a location with an orientation
	DuckEntity(const glm::vec3& position, const glm::quat& orientation);

	std::shared_ptr<DuckMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

private:
	// All entities must have a material with a shader when created
	// This is that material
	static inline std::shared_ptr<DuckMaterial> default_mat;

	// Inherited via BaseEntity
	virtual const BaseMaterial& init_material() override;
};