#pragma once

#include "BaseEntity.h"

struct DuckMaterial : public BaseMaterial
{
	static constexpr const char* MAT_COLOR = "u_color";
	static constexpr const char* TEX_DIFFUSE = "tex_diffuse";

	glm::vec3 color{};
	Texture diffuse;

	DuckMaterial();
protected:
	// Inherited via BaseMaterial
	virtual void bind() const override;
};

class DuckEntity : public BaseEntity
{
public:
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