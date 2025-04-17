#include "window.hpp"
#include "sdl_ext/midiplayer.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

class App {
	WindowWrapper windowWrapper;
	SDL_Window* const& window = windowWrapper.getWindow();
	SDL_Renderer* const& renderer = windowWrapper.getRenderer();

	SDL_MidiPlayer player = SDL_MidiPlayer(windowWrapper.getSpeaker());

	tsf* soundFont = player.loadFont("../pianotiles2.sf2");

public:
	bool running = true;
	float targetFps = 60.0f;
	uint64_t lastTime; // Updated by main.cpp
	float fps; // Updated by main.cpp
	float delta; // Updated by main.cpp
	App() = delete;
	App(App&) = delete;
	App(int argc, char* argv[]) {
		(void)argc;
		(void)argv;
		std::printf("App start!\n");
	}
	~App() {
		std::printf("App quit!\n");
	}
	void frame() {
		// std::printf("Frame: %f %f\n", fps, delta);
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		// render();
		SDL_RenderPresent(renderer);
	}
	void event(const SDL_Event& event) {
		std::printf("Got event %d!\n", event.type);
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			player.play(soundFont, 60);
			printf("hi\n");
		}
	}
};
