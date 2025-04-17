#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>

#include "../lib/midiplayer.hpp"
#include "../lib/error.hpp"

class SDL_MidiPlayer : public MidiPlayer {
private:
	static constexpr size_t SAMPLE_COUNT = 2048;
	static constexpr size_t MAX_BUFFER_SIZE = SAMPLE_COUNT * SAMPLE_SIZE;
	static constexpr float MAX_BUFFER_TIME = (float)SAMPLE_COUNT / (float)SAMPLE_RATE;

	SDL_AudioStream* streamOutput;
	void* streamBuffer = std::malloc(MAX_BUFFER_SIZE);
	std::mutex working;
	size_t blankSamplesToWrite = SAMPLE_COUNT * 2;
	[[gnu::always_inline]] inline void worker() {
		std::unique_lock<std::mutex> lock(working, std::try_to_lock);
		if(!lock.owns_lock())
			return;
		size_t samplesQueued = SDL_GetAudioStreamQueued(streamOutput) / SAMPLE_SIZE;
		if (samplesQueued >= SAMPLE_COUNT)
			return;
		size_t samples = SAMPLE_COUNT - samplesQueued;
		if (blankSamplesToWrite > 0) {
			std::memset(streamBuffer, 0, samples * SAMPLE_SIZE);
			if (samples >= blankSamplesToWrite)
				blankSamplesToWrite = 0;
			else
				blankSamplesToWrite -= samples;
		} else {
			update(streamBuffer, samples);
		}
		SDL_PutAudioStreamData(streamOutput, streamBuffer, samples * SAMPLE_SIZE);
	}
	static void workerWrapper(void* user, SDL_AudioStream* _, int additional, int total) {
		(void)additional;
		(void)total;
		((SDL_MidiPlayer*)user)->worker();
	}

public:
	SDL_MidiPlayer(SDL_AudioDeviceID devid) {
		SDL_AudioSpec spec;
		spec.format = SAMPLE_TYPE == SampleType::SHORT ? SDL_AUDIO_S16 : SDL_AUDIO_F32;
		spec.channels = CHANNELS;
		spec.freq = SAMPLE_RATE;
		streamOutput = SDL_OpenAudioDeviceStream(devid, &spec, workerWrapper, this);
		if(!streamOutput)
			raiseException("SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
		SDL_ResumeAudioStreamDevice(streamOutput);
	}
	~SDL_MidiPlayer() {
		SDL_DestroyAudioStream(streamOutput);
		std::free(streamBuffer);
	}
};
