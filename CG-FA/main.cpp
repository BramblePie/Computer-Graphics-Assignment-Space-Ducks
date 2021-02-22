#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>

#include "glsl.h"

#include "Keybinding.h"
#include "Player.h"
#include "Scene.h"
#include "Entities.h"

// Main window
GLFWwindow* window = 0;

GLFWwindow* CreateWindow();

Scene* LoadScene();

int main()
{
	window = CreateWindow();

	// Setup keybindings on glfw window
	Keybinding::SetTargetWindow(window);
	auto& binding = Keybinding::GetInstance();

	// Load scene with all assets
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

		binding.ProcessEvents();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->RenderLoop(delta);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << (void*)err << std::endl;
			//throw err;
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

	glfwWindowHint(GLFW_SAMPLES, 16);

	/* Create a windowed mode window and its OpenGL context */
	win = glfwCreateWindow(INITIAL::WINDOW_WIDTH, INITIAL::WINDOW_HEIGHT, "Welcome to OpenGL", NULL, NULL);
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
	// Create new scene
	Scene* scene = new Scene();

	// Add lights to scene
	scene->lights.emplace_back(glm::vec3(1.0f, 2.0f, .4f), glm::vec3(1.0f));

	// Add entities to scene

	auto& duck = scene->AddEntity(new DuckEntity(glm::vec3(.0f, 1.2f, .0f)));
	duck.Animate = [&](const float d) {
		duck.orientation = glm::rotate(duck.orientation, d * 1.0f, WORLD::UP);
	};

	auto& right_duck = scene->AddEntity(new DuckEntity(glm::vec3(-.3f, 1.2f, .0f)));

	//auto& ped = scene->AddEntity(new PedestalEntity(glm::vec3(.0f, .0f, .0f)));
	scene->AddEntity(new TileEntity(glm::vec3(.0f, 1.6f, .0f)));

	right_duck.material = std::make_shared<DuckMaterial>(*duck.material);
	right_duck.material->roughness = 0.1f;
	duck.material->metallic = 0.0f;
	//duck.material->roughness = 0.3f;

	return scene;
}