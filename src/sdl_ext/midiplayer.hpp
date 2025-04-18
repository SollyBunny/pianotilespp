#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>

#include <mutex>

#include "../lib/midiplayer.hpp"

class SDL_MidiPlayer : public MidiPlayer {
private:
	static constexpr size_t SAMPLE_COUNT = 2048;
	static constexpr size_t MAX_BUFFER_SIZE = SAMPLE_COUNT * SAMPLE_SIZE;
	static constexpr float MAX_BUFFER_TIME = (float)SAMPLE_COUNT / (float)SAMPLE_RATE;

	SDL_AudioStream* streamOutput;
	void* streamBuffer;
	std::mutex working; // Prevent SDL from spawning multiple workers at the same time
	size_t blankSamplesToWrite = SAMPLE_COUNT * 2; // Silence starting random noise
	[[gnu::always_inline]] inline void worker();
	static void workerWrapper(void* user, SDL_AudioStream* _, int additional, int total);

public:
	SDL_MidiPlayer(SDL_AudioDeviceID devid);
	~SDL_MidiPlayer();
};
