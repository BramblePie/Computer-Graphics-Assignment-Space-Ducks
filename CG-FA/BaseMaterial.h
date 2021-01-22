#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

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
	virtual void bind() const = 0;

private:
	// Currently bound material
	static inline const BaseMaterial* CURR_MAT = 0;
};
