#pragma once

#include <memory>

#include <glm/glm.hpp>
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
		unsigned int vbos[3]{};
	};

	size_t VertexCount = 0;

	~VertexArray();
};

class BaseEntity
{
public:
	static constexpr const char* POS_ATTRIB_NAME = "v_pos";
	static constexpr const char* NORMAL_ATTRIB_NAME = "v_normal";
	static constexpr const char* UV_ATTRIB_NAME = "v_uv";
	static constexpr const char* MODEL_UNIFORM_NAME = "u_model";

	DebugMaterial material;

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat orientation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1.0f);

	/// <summary>
	/// Base constructor for an Entity object
	/// </summary>
	/// <param name="uniqueStr">Either file path to 3D model or a unique name</param>
	BaseEntity(const char* uniqueStr);
	virtual ~BaseEntity()
	{
		// Only remove vertex array it isnt going to be used anymore
		if (BUFFER_CACHE[unique_key].use_count() <= 2)
			BUFFER_CACHE.erase(unique_key);
	}

	inline glm::mat4 GetModel() const
	{
		return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1.0f), scale);
	}

	void Draw();

protected:
	static inline std::unordered_map<std::string, std::shared_ptr<VertexArray>> BUFFER_CACHE;

	const std::string unique_key;
	std::shared_ptr<VertexArray> vao;

	virtual void draw() = 0;
private:
};
