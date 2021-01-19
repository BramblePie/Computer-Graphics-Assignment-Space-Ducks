#include "BasicEntity.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glsl.h"

BasicEntity::BasicEntity()
{
	// Setup material
	material.color = glm::vec3(0.7f, 0.1f, 0.7f);
	if (material.shader = BaseMaterial::SHADER_CACHE["basic_entity"];
		material.shader == 0)
	{	// Create shader if needed
		char* vertexshader = glsl::readFile(R"(Shaders\BasicVertex.shader)");
		char* fragshader = glsl::readFile(R"(Shaders\BasicFragment.shader)");

		material.shader = glsl::makeShaderProgram(
			glsl::makeVertexShader(vertexshader),
			glsl::makeFragmentShader(fragshader));

		// Cache new shader
		BaseMaterial::SHADER_CACHE["basic_entity"] = material.shader;
	}

	// TODO :: parameters needed
	std::vector<glm::vec3> vertices =
	{
		{ -0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.5f, 0.0f },
	};
	std::vector<glm::vec3> colors =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	// END TODO

	// Generate 1 vertex array
	glGenVertexArrays(1, &vao.ID);
	glBindVertexArray(vao.ID);
	vao.VertexCount = vertices.size();

	// Generate new vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	// Bind position data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 vertices.size() * sizeof(decltype(vertices[0])),
				 &vertices[0], GL_STATIC_DRAW);
	vao.VBOs.emplace_back(vbo);

	// Set vertex attribute position
	int attribLoc;
	attribLoc = glGetAttribLocation(material.shader, material.POS_ATTRIB_NAME);
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc, vertices[0].length(), GL_FLOAT, GL_FALSE, 0, 0);

	// Generate 1 vertex array for color
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 colors.size() * sizeof(decltype(colors[0])),
				 &colors[0], GL_STATIC_DRAW);
	vao.VBOs.emplace_back(vbo);

	// Set vertex color attribute
	attribLoc = glGetAttribLocation(material.shader, material.CLR_ATTRIB_NAME);
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc, colors[0].length(), GL_FLOAT, GL_FALSE, 0, 0);
}

void BasicEntity::Draw()
{
	glUseProgram(material.shader);
	glBindVertexArray(vao.ID);
	glDrawArrays(GL_TRIANGLES, 0, vao.VertexCount);
}