#pragma once
#include "../component.hpp"

#include <list>

class Note {
public:
	int lane;
	int note;
	bool joined;
	float delta;
	float ttl;
};

class ComponentGame : public Component {
private:
	std::list<Note> notes;

public:
	ComponentGame(App const& appWith) : Component(appWith) {

	}
	bool event(const SDL_Event& event) override { return false; }
	void frame() override {

	}
};
