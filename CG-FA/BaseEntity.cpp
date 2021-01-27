#include "BaseEntity.h"

#include <iostream>

#include <GL/glew.h>
#include "objloader.h"

// Local function to calculate additional tangents
void calcTangents(std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& pos,
				  std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

BaseEntity::BaseEntity(const char* uniqueStr, const BaseMaterial* material)
	: unique_key(uniqueStr)
{
	if (BUFFER_CACHE[unique_key] == 0)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loadOBJ(uniqueStr, vertices, uvs, normals);

		vao = std::make_shared<VertexArray>();
		glGenVertexArrays(1, &vao->ID);
		glBindVertexArray(vao->ID);

		vao->VertexCount = vertices.size();
		int attribLoc;

		// Generate 5 buffers for position, uv and normal
		glGenBuffers(5, vao->vbos);

		// Bind position buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.position);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		// Set vertex attribute for position
		attribLoc = glGetAttribLocation(material->shader, POS_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind normal buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.normal);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		// Set vertex attribute for normal
		attribLoc = glGetAttribLocation(material->shader, NORMAL_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_TRUE, 0, 0);
		}

		// Bind uv buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.uv);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		// Set vertex attribe for uv
		attribLoc = glGetAttribLocation(material->shader, UV_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Prep list of vectors
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		calcTangents(uvs, vertices, tangents, bitangents);

		// Bind tangent buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.tangent);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

		// Set vertex attribe for tangent
		attribLoc = glGetAttribLocation(material->shader, TANG_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Bind bitangent buffer
		glBindBuffer(GL_ARRAY_BUFFER, vao->buffers.bitangent);
		glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
		// Set vertex attribe for tangent
		attribLoc = glGetAttribLocation(material->shader, BITANG_ATTRIB_NAME);
		if (attribLoc > -1)
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		BUFFER_CACHE[unique_key] = vao;
	}
	else
		vao = BUFFER_CACHE[unique_key];

	std::cout << "Entity constructed from " << uniqueStr;
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
	glUniformMatrix4fv(glGetUniformLocation(GetMaterial()->shader, MODEL_UNIFORM_NAME),
					   1, GL_FALSE, &GetModel()[0][0]);
	draw();
	// Bind material and VAO
	GetMaterial()->Bind();
	glBindVertexArray(vao->ID);
	// Draw all vertices in VAO
	glDrawArrays(GL_TRIANGLES, 0, vao->VertexCount);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(3, vbos);
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
			(pos[i + 1] - pos[i]),
			(pos[i + 2] - pos[i])
		) * glm::inverse(glm::mat2(
			(uvs[i + 1] - uvs[i]),
			(uvs[i + 2] - uvs[i])
		));

		tangents.insert(std::end(tangents), 3, (BT[0]));
		bitangents.insert(std::end(bitangents), 3, (BT[1]));
		//auto t = glm::dot(BT[0], BT[1]);
		//float l0 = glm::length(BT[0]);
		//float l1 = glm::length(BT[1]);
	}
}