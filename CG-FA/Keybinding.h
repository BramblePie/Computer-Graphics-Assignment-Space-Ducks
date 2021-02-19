#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>

#include <GLFW/glfw3.h>

class IKeyObserver;
class ICursorObserver;
class IWindowObserver;

class Keybinding
{
public:
	// Function called with delta time
	using Event = std::function<void(const float)>;

	// Sets the target window to the keybinding will listen
	static inline void SetTargetWindow(GLFWwindow* window)
	{
		Keybinding::target = window;
	}

	// Gets THE keybinding instance
	static inline Keybinding& GetInstance()
	{
		static Keybinding binding;
		return binding;
	}

	// Calls all pending events at the start of a new frame
	void ProcessEvents();

	// Subscribe a new key observer
	void Subscribe(IKeyObserver* observer);
	// Subscribe a new cursor observer
	void Subscribe(ICursorObserver* observer);
	// Subscribe a new window observer
	void Subscribe(IWindowObserver* observer);

private:
	// Target window to listen to
	static inline GLFWwindow* target = 0;

	// All subscribed events per key (int)
	std::unordered_map<int, std::vector<Event>> key_subs;
	// List of all key observers
	std::vector<IKeyObserver*> key_observers;
	// List of all cursor observers
	std::vector<ICursorObserver*> cursor_observers;
	// List of all window observers
	std::vector<IWindowObserver*> window_observers;

	// Set of all currently pressed keys
	mutable std::unordered_set<int> pressed_keys;

	// Privately construct a keybinding instance
	Keybinding();

	Keybinding(const Keybinding&) = delete;	// No copies allowed
	void operator=(const Keybinding&) = delete;
	Keybinding(Keybinding&&) = delete;		// Nor moves
	void operator=(Keybinding&&) = delete;

	// Callback function on each keyboard action
	void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) const;

	// Callback function on each cursor movement
	void cursorCallback(GLFWwindow* window, const float xpos, const float ypos) const;

	// Callback function on window size changes
	void windowSizeCallback(GLFWwindow* window, const float width, const float height) const;
};

// Key observer interface, inherit to subscribe
class IKeyObserver
{
protected:
	// Subscribe all keyobservers to keybinding in default constructor
	IKeyObserver()
	{
		Keybinding::GetInstance().Subscribe(this);
	}

public:
	// Called every frame while a key is pressed
	virtual void OnKeyEvent(const int key) = 0;
	// Called once on key press
	virtual void OnKeyPress(const int key) = 0;
};

// Cursor observer interface, inherit to subscribe
class ICursorObserver
{
protected:
	ICursorObserver()
	{
		Keybinding::GetInstance().Subscribe(this);
	}

public:
	// Called immediately on any cursor movement
	virtual void OnCursorMovement(const float dx, const float dy) = 0;
};

// Window observer interface, inherit to subscribe
class IWindowObserver
{
protected:
	// Subscribe all window observers to keybinding in default constructor
	IWindowObserver()
	{
		Keybinding::GetInstance().Subscribe(this);
	}

public:
	// Called immediately on any change in size of window
	virtual void OnWindowResize(const float width, const float height) = 0;
};