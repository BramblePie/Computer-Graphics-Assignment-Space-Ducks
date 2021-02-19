#include "Keybinding.h"

#include "Constants.h"

void Keybinding::ProcessEvents()
{
	// For every pressed key call all observers
	for (int key : pressed_keys)
		for (auto obs : key_observers)
			obs->OnKeyEvent(key);
}

void Keybinding::Subscribe(IKeyObserver* observer)
{
	key_observers.emplace_back(observer);
}

void Keybinding::Subscribe(ICursorObserver* observer)
{
	cursor_observers.emplace_back(observer);
}

void Keybinding::Subscribe(IWindowObserver* observer)
{
	window_observers.emplace_back(observer);
}

Keybinding::Keybinding()
{
	// Set cursor input settings
	// Don't show cursor in window
	glfwSetInputMode(target, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Try to enable raw mouse capture ( no acceleration )
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(target, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	// Set user pointer to this instance of KeyBinding
	// so this instance can be called inside a lambda
	glfwSetWindowUserPointer(target, this);

	// Set callback functions
	// Cast user pointer to KeyBinding* at compile
	auto keycb = [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
		const static Keybinding* k = static_cast<Keybinding*>(glfwGetWindowUserPointer(window));
		k->keyCallback(window, key, scancode, action, mods);
	};
	glfwSetKeyCallback(target, keycb);

	auto curcb = [](GLFWwindow* window, const double xpos, const double ypos) {
		const static Keybinding* k = static_cast<Keybinding*>(glfwGetWindowUserPointer(window));
		k->cursorCallback(window, static_cast<float>(xpos), static_cast<float>(ypos));
	};
	glfwSetCursorPosCallback(target, curcb);

	auto wincb = [](GLFWwindow* window, const int width, const int height) {
		const static Keybinding* k = static_cast<Keybinding*>(glfwGetWindowUserPointer(window));
		k->windowSizeCallback(window, static_cast<float>(width), static_cast<float>(height));
	};
	glfwSetWindowSizeCallback(target, wincb);
	// "static_cast<Keybinding*>(glfwGetWindowUserPointer(window))" same as "this"

	// Clean up the glfw window user pointer
	// All callback functions need to be called once to initialize the local static variable,
	// that is filled with the window user pointer
	keycb(target, -1, -1, -1, -1);
	curcb(target, INITIAL::WINDOW_WIDTH / 2, INITIAL::WINDOW_HEIGHT / 2);
	wincb(target, INITIAL::WINDOW_WIDTH, INITIAL::WINDOW_HEIGHT);
	glfwSetWindowUserPointer(target, 0);
}

// Callback function on each keyboard action
void Keybinding::keyCallback(GLFWwindow* window, const int key, const int, const int action, const int) const
{
	if (action == GLFW_PRESS)
	{
		// Send window close flag on escape key press
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		// Run all on key press events
		for (auto obs : key_observers)
			obs->OnKeyPress(key);
		// then add keys to pressed keys
		pressed_keys.insert(key);
	}
	else if (action == GLFW_RELEASE)
		pressed_keys.erase(key);
}

void Keybinding::cursorCallback(GLFWwindow*, const float xpos, const float ypos) const
{
	static float _x = xpos, _y = ypos;
	const float dx = xpos - _x;
	const float dy = _y - ypos;
	_x = xpos; _y = ypos;
	for (auto obs : cursor_observers)
		obs->OnCursorMovement(dx, dy);
}

void Keybinding::windowSizeCallback(GLFWwindow*, const float width, const float height) const
{
	for (auto obs : window_observers)
		obs->OnWindowResize(width, height);
}