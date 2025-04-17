#pragma once

// Convienience functions to create streams for devices

#include <SDL3/SDL_audio.h>

[[nodiscard]] inline SDL_AudioStream* SDL_CreateAudioStream(SDL_AudioSpec* src_spec, SDL_AudioDeviceID devid) {
	// Get device format
	SDL_AudioSpec dst_spec;
	[[maybe_unused]] int sampleCount;
	if (!SDL_GetAudioDeviceFormat(devid, &dst_spec, &sampleCount))
		return nullptr;
	// Create stream
	return SDL_CreateAudioStream(src_spec, &dst_spec);
}

[[nodiscard]] inline SDL_AudioStream* SDL_CreateAudioStream(SDL_AudioDeviceID devid, SDL_AudioSpec* dst_spec) {
	// Get device format
	SDL_AudioSpec src_spec;
	[[maybe_unused]] int sampleCount;
	if (!SDL_GetAudioDeviceFormat(devid, &src_spec, &sampleCount))
		return nullptr;
	// Create stream
	return SDL_CreateAudioStream(&src_spec, dst_spec);
}
