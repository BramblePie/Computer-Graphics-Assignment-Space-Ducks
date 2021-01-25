#include "BaseEntity.h"

#include <iostream>

#include <GL/glew.h>
#include "objloader.h"

BaseEntity::BaseEntity(const char* uniqueStr, const BaseMaterial* material)
	: unique_key(uniqueStr), material(material)
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

		// Generate 3 buffers for position, uv and normal
		glGenBuffers(3, vao->vbos);

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

const BaseMaterial* BaseEntity::GetMaterial() const
{
	return material;
}

void BaseEntity::Draw()
{
	glUniformMatrix4fv(glGetUniformLocation(material->shader, MODEL_UNIFORM_NAME),
					   1, GL_FALSE, &GetModel()[0][0]);
	draw();
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &ID);
}