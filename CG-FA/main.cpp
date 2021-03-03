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

	const auto blue = glm::vec3(.4, .4f, 1.0f);
	const auto red = glm::vec3(.5f, .1f, .1f);
	const auto green = glm::vec3(.1f, .5f, .1f);

	// Add lights to scene
	scene->lights.emplace_back(glm::vec3(.0f, 6.0f, 20.0f), 400.0f * blue);
	scene->lights.emplace_back(glm::vec3(.0, 1.6f, .0f), red);
	scene->lights.emplace_back(glm::vec3(.0f, 1.6f, .0f), green);

	auto& blue_light = scene->lights[0];
	auto& red_light = scene->lights[1];
	auto& green_light = scene->lights[2];

	// Animate lighting
	blue_light.Move = [&blue_light](const float delta) {
		static const auto off = blue_light.position;
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = 16.0f;
		// Amplitude ( radius )
		const float a = .2f;
		blue_light.position.x = a * glm::cos(w * glm::radians(360.0f) * t) + off.x;
		blue_light.position.z = -a * glm::sin(w * glm::radians(360.0f) * t) + off.z;
		t += delta;
		if (t > 1.0f / w)
			t = .0f;
	};
	StarEntity& star_blue = scene->AddEntity(new StarEntity(blue_light.position));
	star_blue.material->color = blue;
	star_blue.scale *= 4.0f;
	star_blue.Animate = [&star_blue](const float delta) {
		static const auto off = star_blue.position;
		static float t = .0f;
		// Frequency ( 1/sec )
		const float w = 16.0f;
		// Amplitude ( radius )
		const float a = .05f;
		star_blue.position.x = a * glm::cos(w * glm::radians(360.0f) * t) + off.x;
		star_blue.position.z = -a * glm::sin(w * glm::radians(360.0f) * t) + off.z;
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
	StarEntity& star_red = scene->AddEntity(new StarEntity(red_light.position));
	star_red.material->color = red;
	star_red.scale *= 0.8f;
	star_red.Animate = [&star_red, &red_light](const float) {
		star_red.position = red_light.position;
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
	StarEntity& star_green = scene->AddEntity(new StarEntity(green_light.position));
	star_green.material->color = green;
	star_green.Animate = [&star_green, &green_light](const float) {
		star_green.position = green_light.position;
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

	DuckMaterial* red_mat = new DuckMaterial({ 2.0f, .0f, .0f });
	DuckMaterial* green_mat = new DuckMaterial({ .0f, 1.0f, .0f });
	DuckMaterial* blue_mat = new DuckMaterial({ .0f, .0f, 2.0f });
	ducks[8]->material = std::shared_ptr<DuckMaterial>(red_mat);
	ducks[6]->material = std::shared_ptr<DuckMaterial>(green_mat);
	ducks[4]->material = std::shared_ptr<DuckMaterial>(blue_mat);

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