#include "midiplayer.hpp"

#include <cstdlib>
#include <cstring>

#include "../lib/exception.hpp"

[[gnu::always_inline]] inline void SDL_MidiPlayer::worker() {
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

void SDL_MidiPlayer::workerWrapper(void* user, SDL_AudioStream* _, int additional, int total) {
	(void)additional;
	(void)total;
	((SDL_MidiPlayer*)user)->worker();
}

SDL_MidiPlayer::SDL_MidiPlayer(SDL_AudioDeviceID devid) {
	streamBuffer = checkFunctionErrno(std::malloc, MAX_BUFFER_SIZE);
	SDL_AudioSpec spec;
	spec.format = SAMPLE_TYPE == SampleType::SHORT ? SDL_AUDIO_S16 : SDL_AUDIO_F32;
	spec.channels = CHANNELS;
	spec.freq = SAMPLE_RATE;
	streamOutput = SDL_OpenAudioDeviceStream(devid, &spec, workerWrapper, this);
	if(!streamOutput)
		raiseException("SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
	SDL_ResumeAudioStreamDevice(streamOutput);
}

SDL_MidiPlayer::~SDL_MidiPlayer() {
	SDL_DestroyAudioStream(streamOutput);
	std::free(streamBuffer);
}
