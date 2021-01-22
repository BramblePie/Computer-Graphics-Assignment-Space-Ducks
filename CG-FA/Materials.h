#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glsl.h"

struct BaseMaterial
{
	static inline std::unordered_map<std::string, GLuint> SHADER_CACHE;

	// Shader intended to use with this material
	GLuint shader = 0;

	void Bind() const
	{
		if (CURR_MAT == this)
			return;
		bind();
		CURR_MAT = this;
	}

protected:
	virtual void bind() const = 0;

private:
	// Currently bound material
	static inline const BaseMaterial* CURR_MAT = 0;
};

struct DebugMaterial : public BaseMaterial
{
	static constexpr const char* MAT_COLOR = "u_color";
	glm::vec3 color = { 0.7f, 0.1f, 0.7f };

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

private:
	// Inherited via BaseMaterial
	virtual void bind() const override
	{
		glUniform3fv(glGetUniformLocation(shader, MAT_COLOR), 1, &color[0]);
	}
};