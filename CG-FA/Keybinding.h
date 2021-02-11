#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>

#include <GLFW/glfw3.h>

class Keybinding
{
public:
	Keybinding(GLFWwindow* window)
	{
		// Set cursor input settings
		// Don't show cursor in window
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Try to enable raw mouse capture ( no acceleration )
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		// Set user pointer to this instance of KeyBinding
		glfwSetWindowUserPointer(window, this);

		// Set callback functions
		// Cast void* from user pointer to KeyBinding* at compile
		glfwSetKeyCallback(
			window,
			[](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
				static_cast<Keybinding*>(glfwGetWindowUserPointer(window))
					->keyCallback(window, key, scancode, action, mods);
			});

		glfwSetCursorPosCallback(
			window,
			[](GLFWwindow* window, const double xpos, const double ypos) {
				static_cast<Keybinding*>(glfwGetWindowUserPointer(window))
					->cursorCallback(window, xpos, ypos);
			});
		// "static_cast<Keybinding*>(glfwGetWindowUserPointer(window))" same as "this"
	}

	void ProcessEvents(const float delta)
	{
		for (int key : pressed_keys)
			for (auto& e : key_subs[key])
				e(delta);
	}

private:
	// Event that needs processing
	using Event = std::function<void(const float)>;

	// All subscribed events per key (int)
	std::unordered_map<int, std::vector<Event>> key_subs;
	std::unordered_set<int> pressed_keys;

	// Callback function on each keyboard action
	void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
	{
		if (action == GLFW_PRESS)
			pressed_keys.insert(key);
		else if (action == GLFW_RELEASE)
			pressed_keys.erase(key);
	}

	// Callback function on each cursor movement
	void cursorCallback(GLFWwindow* window, const double xpos, const double ypos) {}
};
