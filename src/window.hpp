#include <SDL3/SDL.h>

#include <cstdio>

#include "lib/exception.hpp"
#include "version.hpp"

class WindowWrapper {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_AudioDeviceID speaker;

public:
	bool valid = false;
	SDL_Window* const& getWindow() const {
		return window;
	}
	SDL_Renderer* const& getRenderer() const {
		return renderer;
	}
	SDL_AudioDeviceID getSpeaker() const {
		return speaker;
	}
	WindowWrapper() {
		checkFunctionSDL(SDL_Init, SDL_INIT_AUDIO | SDL_INIT_VIDEO);
		checkFunctionSDL(SDL_SetAppMetadata, PROJECT_NAME_READABLE, VERSION, PROJECT_IDEN);
		// Create window
		const int flags = SDL_WINDOW_RESIZABLE;
		checkFunctionSDL(SDL_CreateWindowAndRenderer, PROJECT_NAME_READABLE, 512, 512, flags, &window, &renderer);
		// Init audio
		SDL_AudioSpec want = {};
		want.channels = 2;
		speaker = checkFunctionSDL(SDL_OpenAudioDevice, SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want);
		checkFunctionSDL(SDL_ResumeAudioDevice, speaker);
		// Done
		valid = true;
	}
	~WindowWrapper() {
		SDL_CloseAudioDevice(speaker);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
};