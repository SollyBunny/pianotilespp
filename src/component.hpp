#pragma once

class App;
union SDL_Event;

class Component {
public:
	App const& app;
	bool active = true;

	// Return whether should propagate
	virtual bool event(const SDL_Event& event) = 0;
	virtual void frame() = 0;

	Component(App const& appWith) : app(appWith) {};
	virtual ~Component() = default;
};
