#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glsl.h"

#include "Scene.h"
#include "Camera.h"

constexpr int WIDTH = 800, HEIGHT = 600;

GLFWwindow* CreateWindow();

Scene* LoadScene();

int main()
{
	GLFWwindow* window = CreateWindow();

	Scene* scene = LoadScene();

	Camera cam = Camera(window);
	cam.position.z = -1.0f;
	cam.position.x = -1.0f;
	cam.orientation = glm::rotate(cam.orientation, glm::radians(45.0f), { 0.0f, 1.0f, 0.0f });

	glm::mat4 view = cam.GetView();
	glm::mat4 proj = cam.GetProjection();

	GLuint s = BaseMaterial::SHADER_CACHE["basic_entity"];
	glUseProgram(s);
	glUniformMatrix4fv(glGetUniformLocation(s, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(s, "projection"), 1, GL_FALSE, &proj[0][0]);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << (void*)err << std::endl;
		throw err;
	}

	auto cam_front = cam.Front();

	float time = 0.0f;
	float lastFrame = 0.0f;
	float delta = 0.0f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		time = (float)glfwGetTime();
		delta = time - lastFrame;
		lastFrame = time;

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		scene->RenderLoop(delta);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete scene;
	glfwTerminate();
	return 0;
}

GLFWwindow* CreateWindow()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Welcome to OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 0;
	}

	// First make the window's context current
	glfwMakeContextCurrent(window);
	// then initialize glew
	// Initialize glew, close program if it fails
	if (GLEW_OK != glewInit())
	{
		glfwTerminate();
		return 0;
	}

	return window;
}

Scene* LoadScene()
{
	Scene* scene = new Scene();

	scene->entities.emplace_back();

	return scene;
}