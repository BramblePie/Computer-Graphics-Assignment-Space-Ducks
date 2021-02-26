#include "SpaceBox.h"

#include <glm/glm.hpp>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glsl.h"

SpaceBox::SpaceBox()
{
	// Create skybox shader
	char* vertexshader = glsl::readFile(R"(Shaders\skybox.vert)");
	char* fragshader = glsl::readFile(R"(Shaders\skybox.frag)");
	shader = glsl::makeShaderProgram(
		glsl::makeVertexShader(vertexshader),
		glsl::makeFragmentShader(fragshader));
	glUseProgram(shader);

	v_pos = glGetAttribLocation(shader, "pos");
	u_skybox = glGetUniformLocation(shader, "skybox");
	u_view = glGetUniformLocation(shader, "u_view");
	u_projection = glGetUniformLocation(shader, "u_projection");

	// Load skybox texture
	glGenTextures(1, &cube.ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube.ID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(sides[i], &width, &height, &nrChannels, 0);
		if (data)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			printf("[ERROR] Failed to load cubemap, side: %s", sides[i]);

		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Generate buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(v_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

SpaceBox::~SpaceBox()
{
	// Delete all data from gpu
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shader);
}

void SpaceBox::DrawSky(const glm::mat4& view, const glm::mat4& projection)
{
	glUseProgram(shader);
	const auto boxview = glm::mat4(glm::mat3(view));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, &boxview[0][0]);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
	glBindVertexArray(vao);
	glUniform1i(u_skybox, cube.unit);
	// Don't write to depth buffer, skybox always in the back
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(skyboxVertices) / (sizeof(float) * 3));
	glDepthMask(GL_TRUE);
}