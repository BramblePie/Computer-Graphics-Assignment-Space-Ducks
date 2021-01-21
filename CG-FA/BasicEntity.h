#pragma once

#include <vector>

#include "Materials.h"

struct VertexArray
{
	// Vertex array object ID
	unsigned int ID = 0;

	// All vertex buffer objects in this VAO
	std::vector<unsigned int> VBOs;

	size_t VertexCount = 0;
};

class BaseEntity
{
public:
	DebugMaterial material;

	BaseEntity() = default;
	virtual ~BaseEntity() {}

	virtual void Draw() = 0;

private:
};

class BasicEntity : public BaseEntity
{
public:
	BasicEntity();
	~BasicEntity() = default;

	// Inherited via BaseEntity
	virtual void Draw() override;

private:
	VertexArray vao;
};
