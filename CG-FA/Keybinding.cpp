#include "Keybinding.h"

void Keybinding::ProcessEvents()
{
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
	glfwSetWindowUserPointer(target, this);

	// Set callback functions
	// Cast void* from user pointer to KeyBinding* at compile
	glfwSetKeyCallback(
		target,
		[](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
			static_cast<Keybinding*>(glfwGetWindowUserPointer(window))
				->keyCallback(window, key, scancode, action, mods);
		});

	glfwSetCursorPosCallback(
		target,
		[](GLFWwindow* window, const double xpos, const double ypos) {
			static_cast<Keybinding*>(glfwGetWindowUserPointer(window))
				->cursorCallback(window, static_cast<float>(xpos), static_cast<float>(ypos));
		});

	glfwSetWindowSizeCallback(
		target,
		[](GLFWwindow* window, const int width, const int height) {
			static_cast<Keybinding*>(glfwGetWindowUserPointer(window))
				->windowSizeCallback(window, static_cast<float>(width), static_cast<float>(height));
		});
	// "static_cast<Keybinding*>(glfwGetWindowUserPointer(window))" same as "this"
}

// Callback function on each keyboard action
void Keybinding::keyCallback(GLFWwindow*, const int key, const int, const int action, const int)
{
	if (action == GLFW_PRESS)
		pressed_keys.insert(key);
	else if (action == GLFW_RELEASE)
		pressed_keys.erase(key);
}

void Keybinding::cursorCallback(GLFWwindow*, const float xpos, const float ypos)
{
	static float _x = xpos, _y = ypos;
	const float dx = xpos - _x;
	const float dy = _y - ypos;
	_x = xpos; _y = ypos;
	for (auto obs : cursor_observers)
		obs->OnCursorMovement(dx, dy);
}

void Keybinding::windowSizeCallback(GLFWwindow*, const float width, const float height)
{
	for (auto obs : window_observers)
		obs->OnWindowResize(width, height);
}