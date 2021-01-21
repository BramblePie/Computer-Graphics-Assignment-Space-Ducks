#include "DuckEntity.h"

#include "objloader.h"

DuckEntity::DuckEntity()
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(R"(C:\Users\bramp\Desktop\duck\duck.obj)", vertices, uvs, normals);

	glGenVertexArrays(1, &vao.ID);
	glBindVertexArray(vao.ID);

	vao.VertexCount = vertices.size();
	int attribLoc;

	// Generate 3 buffers for position, uv and normal
	glGenBuffers(3, vao.vbos);

	// Bind position buffer
	glBindBuffer(GL_ARRAY_BUFFER, vao.buffers.position);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Set vertex attribute for position
	attribLoc = glGetAttribLocation(material.shader, material.POS_ATTRIB_NAME);
	if (attribLoc > -1)
	{
		glEnableVertexAttribArray(attribLoc);
		glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	else
		printf("Vertex attribute %s failed\n", material.POS_ATTRIB_NAME);

	// Bind normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, vao.buffers.normal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	// Set vertex attribute for normal
	attribLoc = glGetAttribLocation(material.shader, material.NORMAL_ATTRIB_NAME);
	if (attribLoc > -1)
	{
		glEnableVertexAttribArray(attribLoc);
		glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_TRUE, 0, 0);
	}
	else
		printf("Vertex attribute %s failed\n", material.NORMAL_ATTRIB_NAME);

	// Bind uv buffer
	glBindBuffer(GL_ARRAY_BUFFER, vao.buffers.uv);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	// Set vertex attribe for uv
	attribLoc = glGetAttribLocation(material.shader, material.UV_ATTRIB_NAME);
	if (attribLoc > -1)
	{
		glEnableVertexAttribArray(attribLoc);
		glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	else
		printf("Vertex attribute %s failed\n", material.UV_ATTRIB_NAME);

	printf("Duck constructed");
}

void DuckEntity::Draw()
{
	glm::vec3 clr = { 0.7f, 0.1f, 0.7f };
	glUniform3fv(1, 1, &clr[0]);
	glBindVertexArray(vao.ID);
	glDrawArrays(GL_TRIANGLES, 0, vao.VertexCount);
}