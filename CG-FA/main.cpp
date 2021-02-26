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

	printf("[INFO] Loading done\n");

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
	scene->lights.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(8.0f, .0f, .0f));
	scene->lights.emplace_back(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(.0f, 8.0f, .0f));
	scene->lights.emplace_back(glm::vec3(0.0f, 1.0f, -3.0f), glm::vec3(.0f, .0f, 8.0f));

	// Save duck refs to change materials
	std::vector<DuckEntity*> ducks;

	// Place duck statues
	auto setStatue = [scene, &ducks](const float& x, const float& y, const float& z) {
		ducks.push_back(&scene->AddEntity(new DuckEntity({ x, y + 1.2f, z })));
		scene->AddEntity(new PedestalEntity({ x, y, z }));
	};
	// 10 of them total
	for (size_t i = 0; i < 5; i++)
	{
		setStatue(2.0f, .0f, (i - 2.0f) * 2.0f);
		setStatue(-2.0f, .0f, (i - 2.0f) * 2.0f);
	}

	// Change material of ducks
	DuckMaterial* mat = new DuckMaterial(true);
	mat->roughness = .1f;
	mat->metallic = 1.0f;
	mat->color = { 245 / 255.0f, 215 / 255.0f, 121 / 255.0f };
	ducks[6]->material = std::make_shared<DuckMaterial>(*mat);

	// Add tiles to floor
	const size_t w = 6, l = 10;
	for (size_t i = 0; i < w; i++)
		for (size_t j = 0; j < l; j++)
			scene->AddEntity(new TileEntity(glm::vec3(i * 1.0f - w / 2.0f + .5f,
													  0.0f,
													  j * 1.0f - l / 2.0f + .5f)));

	return scene;
}