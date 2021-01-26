#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glsl.h"

#include "BaseMaterial.h"

struct DebugMaterial : public BaseMaterial
{
	static constexpr const char* MAT_COLOR = "u_color";

	glm::vec3 color{};

	DebugMaterial()
	{
		if (shader = BaseMaterial::SHADER_CACHE["debugging"];
			shader == 0)
		{	// Create debug shader if needed
			char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
			char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

			shader = glsl::makeShaderProgram(
				glsl::makeVertexShader(vertexshader),
				glsl::makeFragmentShader(fragshader));

			// Cache new debug shader
			BaseMaterial::SHADER_CACHE["debugging"] = shader;
		}
	}

protected:
	// Inherited via BaseMaterial
	virtual void bind() const override
	{
		glUniform3fv(glGetUniformLocation(shader, MAT_COLOR), 1, &color[0]);
	}
};
