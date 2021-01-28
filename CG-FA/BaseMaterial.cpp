#include "BaseMaterial.h"

// Get cached uniform location inside the shader of this material

int BaseMaterial::GetUniformLocation(const char* name) const
{
	auto i = UNIFORM_CACHE.find(name);
	int loc;
	if (i == std::end(UNIFORM_CACHE))
	{
		// Get unknown location from shader and cache it
		loc = glGetUniformLocation(shader, name);
		UNIFORM_CACHE[name] = loc;
	}
	else	// Get cached location
		loc = i->second;

	return loc;
}