#include "lib/exception.hpp"
#include "lib/log.hpp"

#include "sdl_ext/spritemaptext.hpp"
#include "sdl_ext/midiplayer.hpp"

#include "window.hpp"

#include <SDL3/SDL.h>

class App {
	WindowWrapper windowWrapper;
	SDL_Window* const& window = windowWrapper.getWindow();
	SDL_Renderer* const& renderer = windowWrapper.getRenderer();

	SDL_MidiPlayer player = SDL_MidiPlayer(windowWrapper.getSpeaker());
	SDL_SpriteMapText text = SDL_SpriteMapText(renderer, "font.png", 16, 16);

	static constexpr const char* SOUNDFONT_PATH = "pianotiles2.sf2";
	tsf* soundFont = player.loadFont(SOUNDFONT_PATH);

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
		info("App start!");
		checkFunctionSDL(SDL_SetTextureScaleMode, text.getTexture(), SDL_SCALEMODE_NEAREST);
		if (!soundFont)
			raiseException("Failed to load %s", SOUNDFONT_PATH);
	}
	~App() {
		info("App quit!");
	}
	void frame() {
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		text.draw(renderer, (int)fps, 2.0f, 2.0f, 16.0f);
		// render();
		SDL_RenderPresent(renderer);
	}
	void event(const SDL_Event& event) {
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
