#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glsl.h"

struct BaseMaterial
{
	static inline std::unordered_map<std::string, GLuint> SHADER_CACHE;
};

struct DebugMaterial : public BaseMaterial
{
	static constexpr const char* POS_ATTRIB_NAME = "v_pos";
	static constexpr const char* NORMAL_ATTRIB_NAME = "v_normal";
	static constexpr const char* UV_ATTRIB_NAME = "v_uv";
	static constexpr const char* CLR_ATTRIB_NAME = "v_color";

	// Shader intended to use with this material
	GLuint shader = 0;

	static constexpr const char* MAT_COLOR = "u_color";
	glm::vec3 color = glm::vec3(1.0f);

	DebugMaterial()
	{
		if (shader = BaseMaterial::SHADER_CACHE["debugging"];
			shader == 0)
		{	// Create shader if needed
			char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
			char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

			shader = glsl::makeShaderProgram(
				glsl::makeVertexShader(vertexshader),
				glsl::makeFragmentShader(fragshader));

			// Cache new shader
			BaseMaterial::SHADER_CACHE["debugging"] = shader;
		}
	}
};