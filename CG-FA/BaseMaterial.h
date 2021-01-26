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

	Texture(const char* path)
	{
		// Generate and bind new texture
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		// Setup parameters and try loading image data
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			// Load image data to gpu with virtual parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
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
};