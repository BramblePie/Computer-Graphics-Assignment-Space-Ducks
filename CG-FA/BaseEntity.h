#pragma once

#include <memory>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Constants.h"
#include "BaseMaterial.h"

struct VertexBuffers
{
	unsigned int position, uv, normal, tangent, bitangent;
};

// Wrapper for vertex and its buffers
struct VertexArray
{
	// Vertex array object ID
	unsigned int ID = 0;

	// All vertex buffer objects in this VAO
	// Nameless union to name the different buffer IDs
	union
	{
		VertexBuffers buffers;
		unsigned int vbos[5]{};
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
	static constexpr const char* TANG_ATTRIB_NAME = "v_tangent";
	static constexpr const char* BITANG_ATTRIB_NAME = "v_bitangent";
	static constexpr const char* MODEL_UNIFORM_NAME = "u_model";

	// Basic properties for every entity

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat orientation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 direction = Front();

	//----------------------------------

	/// <summary>
	/// Base constructor for an Entity object
	/// </summary>
	/// <param name="uniqueStr">Either file path to 3D model or a unique name</param>
	BaseEntity(const char* uniqueStr, const BaseMaterial& material);

	virtual ~BaseEntity();

	// Get mvp model from position, orientation and scale
	glm::mat4 GetModel() const;

	// Get base material
	virtual const BaseMaterial* GetMaterial() const = 0;

	// Animation function can be defined by a lambda and will be called before drawing
	std::function<void(const float)> Animate;

	// Start drawing this entity
	void Draw();

	constexpr glm::vec3 Front()	const { return orientation * WORLD::FRONT; }
	constexpr glm::vec3 Back()	const { return orientation * WORLD::BACK; }
	constexpr glm::vec3 Up()	const { return orientation * WORLD::UP; }
	constexpr glm::vec3 Down()	const { return orientation * WORLD::DOWN; }
	constexpr glm::vec3 Left()	const { return orientation * WORLD::LEFT; }
	constexpr glm::vec3 Right()	const { return orientation * WORLD::RIGHT; }

protected:
	using BufferMap = std::unordered_map<std::string, std::shared_ptr<VertexArray>>;
	// Cache for all vertex array objects to prevent loading the same object multiple times
	static inline BufferMap BUFFER_CACHE;

	// A unique string to identify VAOs by entity, e.g. file path
	const std::string unique_key;
	std::shared_ptr<VertexArray> vao;

	// Specialised method to initialize entity material
	virtual const BaseMaterial& init_material() = 0;
};
