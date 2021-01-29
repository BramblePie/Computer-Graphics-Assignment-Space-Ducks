#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct BaseMaterial
{
	// Cache to track all different shaders
	static inline std::unordered_map<std::string, GLuint> SHADER_CACHE;

	// Shader intended to use with this material
	GLuint shader = 0;

	// Bind all properties of this material to the currently bound shader
	void Bind() const;

#pragma region Set uniforms

	template<class UniType>
	void SetUniform(const UniType& v, const char* name)  const { static_assert(false, "Uniform type not supported"); }

	template<>
	void SetUniform<glm::vec3>(const glm::vec3& v, const char* name) const
	{
		glUniform3fv(GetUniformLocation(name), 1, &v[0]);
	}

	template<>
	void SetUniform<GLenum>(const GLenum& v, const char* name) const
	{
		glUniform1i(GetUniformLocation(name), v);
	}

	template<>
	void SetUniform<glm::mat4>(const glm::mat4& v, const char* name) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &v[0][0]);
	}

#pragma endregion

protected:
	// Bind all extensions of material
	virtual void bind() const = 0;

private:
	// Currently bound material
	static inline const BaseMaterial* CURR_MAT = 0;

	// Cache to store all uniform locations of this material
	mutable std::unordered_map<const char*, GLint> UNIFORM_CACHE;

	// Get cached uniform location inside the shader of this material
	int GetUniformLocation(const char* name) const;
};

struct Texture
{
	GLuint ID;
	GLenum unit;

	Texture(const char* path);

	~Texture()
	{
		glDeleteTextures(1, &ID);
	}

private:
	static inline GLenum unit_count = 0;
};