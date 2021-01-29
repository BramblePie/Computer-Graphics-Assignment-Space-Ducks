#include "DuckEntity.h"

#include "objloader.h"
#include "glsl.h"

DuckEntity::DuckEntity()
	: BaseEntity(R"(C:\Users\bramp\Desktop\duck\duck.obj)", init_material()), material(default_mat)
{
	material->color = { 0.9f, 0.2f, 0.8f };
}

const BaseMaterial* DuckEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<DuckMaterial>();
	material = default_mat;

	return material.get();
}

const BaseMaterial* DuckEntity::GetMaterial() const
{
	return material.get();
}

// Inherited via BaseMaterial

DuckMaterial::DuckMaterial() : diffuse(R"(C:\Users\bramp\Desktop\duck\duck_diffuse.png)")
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

void DuckMaterial::bind() const
{
	SetUniform(color, MAT_COLOR);
	SetUniform(diffuse.unit, TEX_DIFFUSE);
}