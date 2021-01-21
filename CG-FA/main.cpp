#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glsl.h"

#include "Scene.h"
#include "Camera.h"

constexpr int WIDTH = 800, HEIGHT = 600;

// Main window
GLFWwindow* window = 0;

GLFWwindow* CreateWindow();

Scene* LoadScene();

int main()
{
	window = CreateWindow();

	Scene* scene = LoadScene();

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << (void*)err << std::endl;
		throw err;
	}

	//auto cam_front = cam.Front();

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
	GLFWwindow* win;

	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* Create a windowed mode window and its OpenGL context */
	win = glfwCreateWindow(WIDTH, HEIGHT, "Welcome to OpenGL", NULL, NULL);
	if (!win)
	{
		glfwTerminate();
		return 0;
	}

	// First make the window's context current
	glfwMakeContextCurrent(win);
	// then initialize glew
	// Initialize glew, close program if it fails
	if (GLEW_OK != glewInit())
	{
		glfwTerminate();
		return 0;
	}

	return win;
}

Scene* LoadScene()
{
	Scene* scene = new Scene(window);

	BasicEntity* basic = new BasicEntity();

	scene->entities[basic->material.shader].push_back(basic);

	return scene;
}