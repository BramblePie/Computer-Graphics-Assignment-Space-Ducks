#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

#include <GL/glew.h>

#include "stb_image.h"

struct BaseMaterial
{
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

protected:
	// Bind all extensions of material
	virtual void bind() const = 0;

private:
	// Currently bound material
	static inline const BaseMaterial* CURR_MAT = 0;
};

struct Texture
{
	GLuint ID;
	GLenum unit;

	Texture(const char* path) : unit(unit_count++)
	{
		// Generate and bind new texture
		glGenTextures(1, &ID);
		glActiveTexture(unit);
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
	static inline GLenum unit_count = GL_TEXTURE0;
};