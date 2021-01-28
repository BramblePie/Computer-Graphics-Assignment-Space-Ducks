#pragma once

#include "BaseEntity.h"

struct DuckMaterial : public BaseMaterial
{
	static constexpr const char* MAT_COLOR = "u_color";
	static constexpr const char* TEX_DIFFUSE = "tex_diffuse";

	glm::vec3 color{};
	Texture diffuse;

	DuckMaterial() : diffuse(R"(C:\Users\bramp\Desktop\duck\duck_diffuse.png)")
	{
		if (shader = BaseMaterial::SHADER_CACHE["duck"];
			shader == 0)
		{	// Create debug shader if needed
			char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
			char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

			shader = glsl::makeShaderProgram(
				glsl::makeVertexShader(vertexshader),
				glsl::makeFragmentShader(fragshader));

			// Cache new debug shader
			BaseMaterial::SHADER_CACHE["duck"] = shader;
		}
	}
protected:
	// Inherited via BaseMaterial
	virtual void bind() const override
	{
		//glUniform3fv(glGetUniformLocation(shader, MAT_COLOR), 1, &color[0]);
		SetUniform(color, MAT_COLOR);
		//glUniform1i(glGetUniformLocation(shader, TEX_DIFFUSE), diffuse.unit);
		SetUniform(diffuse.unit, TEX_DIFFUSE);
	}
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