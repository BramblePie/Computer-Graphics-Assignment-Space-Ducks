#include "BaseEntity.h"

#include <iostream>

#include <GL/glew.h>
#include "objloader.h"

#include "BaseMaterial.h"

// Local function to calculate additional tangents
void calcTangents(std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& pos,
				  std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

BaseEntity::BaseEntity(const char* uniqueStr, const BaseMaterial& material)
	: unique_key(uniqueStr)
{
	// Check if entity is already cached
	if (BUFFER_CACHE[unique_key] == 0)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		// Create std vectors and load vertex buffers
		loadOBJ(uniqueStr, vertices, uvs, normals);

		// Create a shared pointer to a new VertexArray object
		vao = std::make_shared<VertexArray>();
		// Generate a new vertex array and bind to it
		glGenVertexArrays(1, &vao->ID);
		glBindVertexArray(vao->ID);

		// Set number of vertices in a buffer
		vao->VertexCount = vertices.size();

		// Generate 5 buffers for position, uv, normal and tangents
		glGenBuffers(5, vao->vbos);

		// Bind position buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.position);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		// Set vertex attribute for position
		int attribLoc = glGetAttribLocation(material.shader, POS_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind normal buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.normal);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		// Set vertex attribute for normal
		attribLoc = glGetAttribLocation(material.shader, NORMAL_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind uv buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.uv);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		// Set vertex attribe for uv
		attribLoc = glGetAttribLocation(material.shader, UV_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Prep list of vectors
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;	// !! not currently used and not loaded
		calcTangents(uvs, vertices, tangents, bitangents);

		// Bind tangent buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.tangent);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

		// Set vertex attribe for tangent
		attribLoc = glGetAttribLocation(material.shader, TANG_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		// Add new and loaded vertex array to cache
		BUFFER_CACHE[unique_key] = vao;
	}
	else
	{
		// If cache is hit, use it
		vao = BUFFER_CACHE[unique_key];
		printf("[INFO] Cache hit for entity %s\n", uniqueStr);
	}

	printf("[INFO] Entity constructed from %s\n", uniqueStr);
}

BaseEntity::~BaseEntity()
{
	// Only remove vertex array it isnt going to be used anymore
	if (BUFFER_CACHE[unique_key].use_count() <= 2)
		BUFFER_CACHE.erase(unique_key);
}

glm::mat4 BaseEntity::GetModel() const
{
	return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1.0f), scale);
}

void BaseEntity::Draw()
{
	GetMaterial()->SetUniform(GetModel(), MODEL_UNIFORM_NAME);
	// Bind material and VAO
	GetMaterial()->Bind();
	glBindVertexArray(vao->ID);
	// Draw all vertices in VAO
	glDrawArrays(GL_TRIANGLES, 0, vao->VertexCount);
}

VertexArray::~VertexArray()
{
	// Delete all buffers
	glDeleteBuffers(5, vbos);
	glDeleteVertexArrays(1, &ID);
}

void calcTangents(std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& pos, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
	tangents.reserve(pos.size());
	bitangents.reserve(pos.size());
	// Take steps of 3 vertices to get whole faces
	for (size_t i = 0; i < pos.size(); i += 3)
	{
		glm::mat2x3 BT = glm::mat2x3(
			glm::normalize(pos[i + 1] - pos[i]),
			glm::normalize(pos[i + 2] - pos[i])
		) * glm::inverse(glm::mat2(
			glm::normalize(uvs[i + 1] - uvs[i]),
			glm::normalize(uvs[i + 2] - uvs[i])
		));

		tangents.insert(std::end(tangents), 3, (BT[0]));
		bitangents.insert(std::end(bitangents), 3, (BT[1]));
	}
}