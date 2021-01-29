#include "BaseMaterial.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void BaseMaterial::Bind() const
{
	// Check if this material is already bound
	if (CURR_MAT == this)
		return;
	// Only bind unbound material
	bind();
	CURR_MAT = this;
}

int BaseMaterial::GetUniformLocation(const char* name) const
{
	auto i = UNIFORM_CACHE.find(name);
	int loc;
	if (i == std::end(UNIFORM_CACHE))
	{
		// Get unknown location from shader and cache it
		loc = glGetUniformLocation(shader, name);
		UNIFORM_CACHE[name] = loc;
		if (loc < 0)
			printf("[WARNING] Uniform %s not found in shader %i via material %p\n", name, shader, this);
	}
	else	// Get cached location
		loc = i->second;

	return loc;
}

Texture::Texture(const char* path) : unit(unit_count++)
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
		printf("[ERROR] Failed to load texture %s", path);

	// Free data from RAM
	stbi_image_free(data);
}