#pragma once

#include "BaseEntity.h"

struct PedestalMaterial : public TexturedMaterial
{
	PedestalMaterial();

	// Inherited via BaseMaterial
	virtual const char* GetShaderName() const override { return "pedestal"; }

protected:
};

class PedestalEntity : public BaseEntity
{
public:
	PedestalEntity(const glm::vec3& position);

	std::shared_ptr<PedestalMaterial> material;

	// Inherited via BaseEntity
	virtual const BaseMaterial* GetMaterial() const override;

protected:

private:
	static inline std::shared_ptr<PedestalMaterial> default_mat;

	// Inherited via BaseEntity
	virtual void draw() override {};
	virtual const BaseMaterial* init_material() override;
};
