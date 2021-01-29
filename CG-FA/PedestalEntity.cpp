#include "PedestalEntity.h"

#include "glsl.h"

PedestalMaterial::PedestalMaterial()
	: diffuse(R"(C:\Users\bramp\Desktop\Pedestal\MarbleDiffTex2K.png)")
{
	const char* shader_name = "pedestal";
	if (shader = BaseMaterial::SHADER_CACHE[shader_name];
		shader == 0)
	{	// Create debug shader if needed
		char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
		char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

		shader = glsl::makeShaderProgram(
			glsl::makeVertexShader(vertexshader),
			glsl::makeFragmentShader(fragshader));

		// Cache new debug shader
		BaseMaterial::SHADER_CACHE[shader_name] = shader;
	}
}

void PedestalMaterial::bind() const
{
	SetUniform(diffuse.unit, TEX_DIFFUSE);
}

PedestalEntity::PedestalEntity(const glm::vec3& position)
	: BaseEntity(R"(C:\Users\bramp\Desktop\Pedestal\Pedestal.obj)", init_material()), material(default_mat)
{
	this->position = position;
}

const BaseMaterial* PedestalEntity::GetMaterial() const
{
	return material.get();
}

const BaseMaterial* PedestalEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<PedestalMaterial>();
	material = default_mat;

	return material.get();
}