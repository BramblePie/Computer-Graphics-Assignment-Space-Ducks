#pragma once

#include <vector>

#include <glm/gtc/quaternion.hpp>

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

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat orientation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1.0f);

	BaseEntity() = default;
	virtual ~BaseEntity() {}

	glm::mat4 GetModel() const
	{
		return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1.0f), scale);
	}

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

class DuckEntity : public BaseEntity
{
public:
	DuckEntity();
	~DuckEntity() = default;

private:
};