#pragma once

#include <vector>

#include <glm/gtc/quaternion.hpp>

#include "Materials.h"

struct VertexBuffers
{
	unsigned int position, uv, normal;
};

struct VertexArray
{
	// Vertex array object ID
	unsigned int ID = 0;

	// All vertex buffer objects in this VAO
	union
	{
		VertexBuffers buffers;
		unsigned int vbos[3];
	};

	size_t VertexCount = 0;

	~VertexArray()
	{
		glDeleteBuffers(3, vbos);
		glDeleteVertexArrays(1, &ID);
	}
};

class BaseEntity
{
public:
	static constexpr const char* POS_ATTRIB_NAME = "v_pos";
	static constexpr const char* NORMAL_ATTRIB_NAME = "v_normal";
	static constexpr const char* UV_ATTRIB_NAME = "v_uv";

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

protected:
	VertexArray vao;

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
};
