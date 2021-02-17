#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>

#include <GLFW/glfw3.h>

class IKeyObserver;
class IWindowObserver;

class Keybinding
{
public:
	// Event that needs processing
	using Event = std::function<void(const float)>;

	static inline void SetTargetWindow(GLFWwindow* window)
	{
		Keybinding::target = window;
	}

	static inline Keybinding& GetInstance()
	{
		static Keybinding binding;
		return binding;
	}

	void ProcessEvents();

	void Subscribe(IKeyObserver* observer);
	void Subscribe(IWindowObserver* observer);

private:
	// Target window to listen to
	static inline GLFWwindow* target = 0;

	// All subscribed events per key (int)
	std::unordered_map<int, std::vector<Event>> key_subs;
	std::vector<IKeyObserver*> key_observers;
	std::vector<IWindowObserver*> window_observers;
	std::unordered_set<int> pressed_keys;

	Keybinding();

	Keybinding(const Keybinding&) = delete;	// No copies allowed
	void operator=(const Keybinding&) = delete;
	Keybinding(Keybinding&&) = delete;		// Nor moves

	// Callback function on each keyboard action
	void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);

	// Callback function on window size changes
	void windowSizeCallback(GLFWwindow* window, const int width, const int height);

	// Callback function on each cursor movement
	void cursorCallback(GLFWwindow* window, const double xpos, const double ypos) {}
};

class IKeyObserver
{
protected:
	// Subscribe all keyobservers to keybinding in default constructor
	IKeyObserver()
	{
		Keybinding::GetInstance().Subscribe(this);
	}

public:
	virtual void OnKeyEvent(const int key) = 0;
};

class IWindowObserver
{
protected:
	// Subscribe all window observers to keybinding in default constructor
	IWindowObserver()
	{
		Keybinding::GetInstance().Subscribe(this);
	}

public:
	virtual void OnWindowResize(const float width, const float height) = 0;
};