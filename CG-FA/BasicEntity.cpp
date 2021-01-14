#include "BasicEntity.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glsl.h"

BasicEntity::BasicEntity()
{
	// Setup material
	material.color = glm::vec3(0.7f, 0.1f, 0.7f);
	if (material.shader = SHADER_CACHE["basic_entity"];
		material.shader == 0)
	{	// Create shader if needed
		char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
		char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

		material.shader = glsl::makeShaderProgram(
			glsl::makeVertexShader(vertexshader),
			glsl::makeFragmentShader(fragshader));

		// Cache new shader
		SHADER_CACHE["basic_entity"] = material.shader;
	}

	// TODO :: parameters needed
	std::vector<glm::vec3> vertices =
	{
		{ -0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.5f, 0.0f },
		{ 0.5f, 0.0f, 0.0f }
	};
	std::vector<unsigned int> indices = { 0, 2, 1 };
	// END TODO

	// Genetate 1 vertex array
	glGenVertexArrays(1, &vao.ID);
	glBindVertexArray(vao.ID);

	// Generate new vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vao.VBOs.emplace_back(vbo);
	// Bind position data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 vertices.size() * sizeof(decltype(vertices[0])),
				 &vertices[0], GL_STATIC_DRAW);

	// Set vertex attribute position
	int attribLoc = glGetAttribLocation(material.shader, material.POS_ATTRIB_NAME);
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc, vertices[0].length(), GL_FLOAT, GL_FALSE, 0, 0);

	// Generate index buffer
	glGenBuffers(1, &vao.IBO);
	vao.IndexCount = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indices.size() * sizeof(decltype(indices[0])),
				 &indices[0], GL_STATIC_DRAW);
}

void BasicEntity::Draw()
{
	glBindVertexArray(vao.ID);
	glDrawElements(GL_TRIANGLES, vao.IndexCount, GL_UNSIGNED_INT, 0);
}