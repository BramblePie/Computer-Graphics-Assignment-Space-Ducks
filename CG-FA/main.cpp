#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glsl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Scene.h"
#include "Camera.h"
#include "DuckEntity.h"

constexpr int WIDTH = 1200, HEIGHT = 800;

// Main window
GLFWwindow* window = 0;

GLFWwindow* CreateWindow();

Scene* LoadScene();

int main()
{
	window = CreateWindow();

	Scene* scene = LoadScene();

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->RenderLoop(delta);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << (void*)err << std::endl;
			throw err;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete scene;
	glfwTerminate();

	// Wait before exiting
	//system("pause");
	return 0;
}

GLFWwindow* CreateWindow()
{
	GLFWwindow* win;

	/* Initialize the library */
	if (!glfwInit())
		return 0;

	glfwWindowHint(GLFW_SAMPLES, 8);

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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	return win;
}

Scene* LoadScene()
{
	Scene* scene = new Scene(window);

	auto& duck = scene->AddEntity(new DuckEntity());

	auto& right_duck = scene->AddEntity(new DuckEntity());

	right_duck.material = std::make_shared<DuckMaterial>(*duck.material);
	right_duck.material->color = glm::vec3(0.3f, 0.2f, 0.99f);
	right_duck.position.x += 0.5f;

	return scene;
}