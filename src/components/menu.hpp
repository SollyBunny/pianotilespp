#pragma once

#include "../component.hpp"

class ComponentMenu : public Component {
public:
	ComponentMenu(App const& appWith) : Component(appWith) {
		active = false;
	}
	bool event(const SDL_Event& event) override { return false; }
	void frame() override;
};
