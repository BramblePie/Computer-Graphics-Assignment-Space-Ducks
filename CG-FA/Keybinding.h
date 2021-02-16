#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>

#include <GLFW/glfw3.h>

class IKeyObserver;

class Keybinding
{
public:
	// Event that needs processing
	using Event = std::function<void(const float)>;

	static inline void SetTargetWindow(GLFWwindow* window)
	{
		Keybinding::window = window;
	}

	static inline Keybinding& GetInstance()
	{
		static Keybinding binding;
		return binding;
	}

	void ProcessEvents(const float delta);

	void Subscribe(IKeyObserver* observer);

private:
	// Target window to listen to
	static inline GLFWwindow* window = 0;

	// All subscribed events per key (int)
	std::unordered_map<int, std::vector<Event>> key_subs;
	std::vector<IKeyObserver*> observers;
	std::unordered_set<int> pressed_keys;

	Keybinding();

	Keybinding(const Keybinding&) = delete;	// No copies allowed
	void operator=(const Keybinding&) = delete;
	Keybinding(Keybinding&&) = delete;		// Nor moves

	// Callback function on each keyboard action
	void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);

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
	virtual void OnEvent(const int key, const float delta) = 0;
};