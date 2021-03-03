#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
	float lastFrame = (float)glfwGetTime();	// First delta is time since start up ( multiple seconds )
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

		//GLenum err;
		//while ((err = glGetError()) != GL_NO_ERROR)
		//{
		//	std::cout << (void*)err << std::endl;
		//	//throw err;
		//}

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

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	scene->lights.emplace_back(glm::vec3(.0f, 12.0f, -32.0f), 800.0f * glm::vec3(.4, .4f, 1.2f));
	scene->lights.emplace_back(glm::vec3(.0, 1.6f, .0f), glm::vec3(.5f, .1f, .1f));
	scene->lights.emplace_back(glm::vec3(.0f, 1.6f, .0f), glm::vec3(.1f, .5f, .1f));

	auto& blink_light = scene->lights[0];
	auto& red_light = scene->lights[1];
	auto& green_light = scene->lights[2];

	StarEntity& star = scene->AddEntity(new StarEntity(blink_light.position));
	star.material->color = glm::vec3(.0f, .0f, 1.0f);
	star.Animate = [&star](const float delta) {
		static const auto off = star.position;
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = 16.0f;
		// Amplitude ( radius )
		const float a = .05f;
		star.position.x = a * glm::cos(w * glm::radians(360.0f) * t) + off.x;
		star.position.z = -a * glm::sin(w * glm::radians(360.0f) * t) + off.z;
		t += delta;
		if (t > 1.0f / w)
			t = .0f;
	};

	// Animate lighting
	blink_light.Move = [&blink_light](const float delta) {
		static const auto off = blink_light.position;
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = 16.0f;
		// Amplitude ( radius )
		const float a = .2f;
		blink_light.position.x = a * glm::cos(w * glm::radians(360.0f) * t) + off.x;
		blink_light.position.z = -a * glm::sin(w * glm::radians(360.0f) * t) + off.z;
		t += delta;
		if (t > 1.0f / w)
			t = .0f;
	};
	red_light.Move = [&red_light](const float delta) {
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = .4f;
		// Amplitude ( radius )
		const float a = 10.0f;
		red_light.position.x = a * glm::cos(w * glm::radians(360.0f) * t);
		red_light.position.z = -a * glm::sin(w * glm::radians(360.0f) * t);
		t += delta;
		if (t > 1.0f / w)
			t = .0f;
	};
	green_light.Move = [&green_light](const float delta) {
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = .1f;
		// Amplitude ( radius )
		const float a = 30.0f;
		green_light.position.x = a * glm::cos(w * glm::radians(360.0f) * t);
		green_light.position.z = a * glm::sin(w * glm::radians(360.0f) * t);
		t += delta;
		if (t > 1.0f / w)
			t = .0f;
	};

	// Save duck refs to change materials
	std::vector<DuckEntity*> ducks;

	// Place duck statues
	auto setStatue = [scene, &ducks](const float& x, const float& y, const float& z, const glm::quat& rot) {
		ducks.push_back(&scene->AddEntity(new DuckEntity({ x, y + 1.2f, z }, rot)));
		scene->AddEntity(new PedestalEntity({ x, y, z }));
	};
	// 10 of them total
	for (size_t i = 0; i < 5; i++)
	{
		setStatue(2.0f, .0f, (i - 2.0f) * 2.0f, glm::rotate(glm::identity<glm::quat>(), glm::radians(-90.0f), WORLD::UP));
		setStatue(-2.0f, .0f, (i - 2.0f) * 2.0f, glm::rotate(glm::identity<glm::quat>(), glm::radians(90.0f), WORLD::UP));
	}

	// Add tiles to floor
	const size_t w = 6, l = 10;
	for (size_t i = 0; i < w; i++)
		for (size_t j = 0; j < l; j++)
			scene->AddEntity(new TileEntity(glm::vec3(i * 1.0f - w / 2.0f + .5f,
													  0.0f,
													  j * 1.0f - l / 2.0f + .5f)));

	// Change material of ducks
	DuckMaterial* gold = new DuckMaterial({ 249 / 255.0f, 166 / 255.0f, 2 / 255.0f },
										  .1f, 1.0f);
	DuckMaterial* silver = new DuckMaterial({ 190 / 255.0f, 194 / 255.0f, 203 / 255.0f },
											.06f, 1.0f);
	DuckMaterial* brons = new DuckMaterial({ 119 / 255.0f, 79 / 255.0f, 46 / 255.0f },
										   .16f, 1.0f);

	ducks[9]->material = std::shared_ptr<DuckMaterial>(gold);
	ducks[7]->material = std::shared_ptr<DuckMaterial>(silver);
	ducks[5]->material = std::shared_ptr<DuckMaterial>(brons);

	DuckMaterial* red = new DuckMaterial({ 2.0f, .0f, .0f });
	DuckMaterial* green = new DuckMaterial({ .0f, 1.0f, .0f });
	DuckMaterial* blue = new DuckMaterial({ .0f, .0f, 2.0f });
	ducks[8]->material = std::shared_ptr<DuckMaterial>(red);
	ducks[6]->material = std::shared_ptr<DuckMaterial>(green);
	ducks[4]->material = std::shared_ptr<DuckMaterial>(blue);

	DuckMaterial* yellow = new DuckMaterial({ 1.0f, 1.0f, .0f }, .1f, .0f);
	DuckMaterial* purple = new DuckMaterial({ 1.0f, .0f, 1.0f }, .15f, .0f);
	ducks[3]->material = std::shared_ptr<DuckMaterial>(yellow);
	ducks[3]->scale = glm::vec3(.3f);
	ducks[2]->material = std::shared_ptr<DuckMaterial>(purple);
	ducks[2]->scale = glm::vec3(2.0f);

	// Animate some ducks
	ducks[0]->Animate = [&greg = *ducks[0]](const float delta) {
		static float t = .0f;
		static const auto init = greg.position;
		// After 3 seconds
		if (t > 5.0f && t <= 9.0f)
		{	// Move to x -2 ( 4 spaces for 4 seconds ) and rotate
			greg.position.x -= delta;
			greg.orientation = glm::rotate(greg.orientation, glm::radians(45.0f) * delta, WORLD::UP);
		}
		else if (t > 14.0f)
		{	// Move back
			greg.position.x += delta;
			greg.orientation = glm::rotate(greg.orientation, glm::radians(45.0f) * delta, WORLD::UP);
			if (t > 18.0f)
			{
				t = .0f;	// reset
				greg.position = init;
				return;
			}
		}
		t += delta;
	};

	// Start in up direction
	ducks[1]->direction = ducks[1]->Up();
	ducks[1]->Animate = [&mark = *ducks[1]](const float delta){
		static float t = .0f;
		static const auto init = mark.position;
		// After 3 seconds
		if (t > 5.0f && t <= 9.0f)
		{	// Move up and over
			mark.direction = glm::rotate(mark.direction, glm::radians(45.0f) * delta, mark.Left());
			mark.position.y += mark.direction.y * delta;
			mark.position.x += delta;
		}
		else if (t > 14.0f)
		{	// Move up and back
			mark.direction = glm::rotate(mark.direction, glm::radians(45.0f) * delta, mark.Right());
			mark.position.y -= mark.direction.y * delta;
			mark.position.x -= delta;
			if (t > 18.0f)
			{	// Reset
				t = .0f;
				mark.direction = mark.Up();
				mark.position = init;
				return;
			}
		}
		t += delta;
	};

	return scene;
}