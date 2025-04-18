#include "loadimage.hpp"

#include "../external/stb_image/stb_image.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

SDL_Texture* SDL_LoadImage(SDL_Renderer* const& renderer, const char* filename) {
	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
	if (!data) {
		SDL_SetError("stbi_load failed: %s", stbi_failure_reason());
		return nullptr;
	}
	if (channels != 3 && channels != 4) {
		SDL_SetError("Image did not have 3 or 4 channels");
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		channels == 4 ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB24,
		SDL_TEXTUREACCESS_STATIC,
		width, height
	);
	if(!texture) {
		stbi_image_free(data);
		return nullptr;
	}
	if(!SDL_UpdateTexture(texture, NULL, data, width * 4)) {
		SDL_DestroyTexture(texture);
		stbi_image_free(data);
		return nullptr;
	}
	stbi_image_free(data);
	return texture;
}

