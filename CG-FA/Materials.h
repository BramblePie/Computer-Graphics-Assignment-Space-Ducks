#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

static std::unordered_map<std::string, GLuint> SHADER_CACHE;

struct DebugMaterial
{
	static constexpr const char* POS_ATTRIB_NAME = "v_pos";
	static constexpr const char* CLR_ATTRIB_NAME = "v_color";

	// Shader intended to use with this material
	GLuint shader = 0;

	static constexpr const char* MAT_COLOR = "u_color";
	glm::vec3 color = glm::vec3(1.0f);

	DebugMaterial() = default;
};