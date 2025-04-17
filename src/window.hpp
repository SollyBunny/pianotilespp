#include <SDL3/SDL.h>

#include <cstdio>

#include "lib/error.hpp"
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
		const int subsystems = SDL_INIT_AUDIO | SDL_INIT_VIDEO;
		if (!SDL_Init(subsystems))
			raiseException("SDL_Init failed: %s", SDL_GetError());
		// Set app metadata
		if (!SDL_SetAppMetadata(PROJECT_NAME_READABLE, VERSION, PROJECT_IDEN))
			raiseException("SDL_SetAppMetadata failed: %s", SDL_GetError());
		// Create window
		const int flags = SDL_WINDOW_RESIZABLE;
		if(!SDL_CreateWindowAndRenderer(PROJECT_NAME_READABLE, 512, 512, flags, &window, &renderer))
			raiseException("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
		// Init audio
		SDL_AudioSpec want = {};
		want.channels = 2;
		speaker = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want);
		if (speaker == 0)
			raiseException("SDL_OpenAudioDevice failed: %s", SDL_GetError());
		SDL_ResumeAudioDevice(speaker);
		// Done
		valid = true;
	}
	~WindowWrapper() {
		SDL_CloseAudioDevice(speaker);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
};