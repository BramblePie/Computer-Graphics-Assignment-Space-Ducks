#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "stb_image.h"

struct BaseMaterial
{
	// Cache to track all different shaders
	static inline std::unordered_map<std::string, GLuint> SHADER_CACHE;

	// Shader intended to use with this material
	GLuint shader = 0;

	// Bind all properties of this material to the currently bound shader
	void Bind() const
	{
		// Check if this material is already bound
		if (CURR_MAT == this)
			return;
		// Only bind unbound material
		bind();
		CURR_MAT = this;
	}

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

	Texture(const char* path) : unit(unit_count++)
	{
		// Generate and bind new texture
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, ID);

		// Setup parameters and try loading image data
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			// Load image data to gpu with virtual parameters
			if (nrChannels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else if (nrChannels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
			std::cout << "Failed to load texture" << std::endl;

		// Free data from RAM
		stbi_image_free(data);
	}

	~Texture()
	{
		glDeleteTextures(1, &ID);
	}

private:
	static inline GLenum unit_count = 0;
};