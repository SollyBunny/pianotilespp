#pragma once

// Convienience functions to create streams for devices

#include <SDL3/SDL_audio.h>

[[nodiscard]] SDL_AudioStream* SDL_CreateAudioStream(SDL_AudioSpec* src_spec, SDL_AudioDeviceID devid);
[[nodiscard]] SDL_AudioStream* SDL_CreateAudioStream(SDL_AudioDeviceID devid, SDL_AudioSpec* dst_spec);
