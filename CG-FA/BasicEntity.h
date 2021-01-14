#pragma once

#include <vector>

#include "Materials.h"

struct VertexArray
{
	// Vertex array object ID
	unsigned int ID;

	// All vertex buffer objects in this VAO
	std::vector<unsigned int> VBOs;

	// Index buffer object
	unsigned int IBO;

	size_t IndexCount;
};

class BasicEntity
{
public:
	DebugMaterial material;

	BasicEntity();
	~BasicEntity() = default;

	void Draw();

private:
	VertexArray vao;
};
