#pragma once

#include "lib/exception.hpp"
#include "lib/log.hpp"

#include "sdl_ext/spritemaptext.hpp"
#include "sdl_ext/midiplayer.hpp"

#include <SDL3/SDL.h>

#include "window.hpp"
#include "component.hpp"

#include "components/game.hpp"
#include "components/menu.hpp"

#include <memory>
#include <vector>

class App {
public:
	bool running = true;
	std::vector<std::unique_ptr<Component>> components;

	float targetFps = 60.0f;
	uint64_t lastTime; // Updated by main.cpp
	float fps; // Updated by main.cpp
	float delta; // Updated by main.cpp

	WindowWrapper windowWrapper;
	SDL_Window* const& window = windowWrapper.getWindow();
	SDL_Renderer* const& renderer = windowWrapper.getRenderer();

	SDL_MidiPlayer player = SDL_MidiPlayer(windowWrapper.getSpeaker());
	SDL_SpriteMapText text = SDL_SpriteMapText(renderer, "font.png", 16, 16);

	static constexpr const char* SOUNDFONT_PATH = "pianotiles2.sf2";
	tsf* soundFont = player.loadFont(SOUNDFONT_PATH);

	App() = delete;
	App(App&) = delete;
	App(int argc, char* argv[]) {
		(void)argc;
		(void)argv;
		info("App start!");
		checkFunctionSDL(SDL_SetTextureScaleMode, text.getTexture(), SDL_SCALEMODE_NEAREST);
		if (!soundFont)
			raiseException("Failed to load %s", SOUNDFONT_PATH);
#define ADD(component) components.push_back(std::make_unique<component>(*this));
		ADD(ComponentGame);
		ADD(ComponentMenu);
#undef ADD
	}
	~App() {
		info("App quit!");
	}
	void frame() {
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		for (std::unique_ptr<Component>& component : components)
			if(component->active)
				component->frame();
		text.draw(renderer, (int)fps, 2.0f, 2.0f, 16.0f);
		SDL_RenderPresent(renderer);
	}
	void event(const SDL_Event& event) {
		for (std::unique_ptr<Component>& component : components)
			if (component->active && component->event(event))
				break;
		switch (event.type) {
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				player.play(soundFont, 60);
				break;
			case SDL_EVENT_KEY_DOWN:
				int note = event.key.key;
				while (note < 20)
					note += 20;
				while (note > 100)
					note -= 20;
				player.play(soundFont, note);
				break;
		}
	}
};
