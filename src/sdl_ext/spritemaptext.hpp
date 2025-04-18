#pragma once

#include <SDL3/SDL.h>

#include "spritemap.hpp"

class SDL_SpriteMapText : public SDL_SpriteMap {
public:
	SDL_SpriteMapText(SDL_Renderer* const& renderer, const char* filename, int tilesWide, int tilesHigh, int tileWidth = 0, int tileHeight = 0)
		: SDL_SpriteMap(renderer, filename, tilesWide, tilesHigh, tileWidth, tileHeight)
	{

	}
	bool draw(SDL_Renderer* const& renderer, char c, float x, float y, float size) {
		int i = (int)(unsigned char)c;
		return SDL_SpriteMap::draw(renderer, i % tilesW, i / tilesW, x, y, size);
	}
	bool draw(SDL_Renderer* const& renderer, const char* text, float x, float y, float size) {
		for (int i = 0; *text != '\0'; ++i, ++text) {
			if(!draw(renderer, *text, x + i * size, y, size))
				return false;
		}
		return true;
	}
	bool draw(SDL_Renderer* const& renderer, int number, float x, float y, float size) {
		if (number == 0)
			return draw(renderer, '0', x, y, size);
		// Handle negative
		if (number < 0) {
			if (!draw(renderer, '-', x, y, size))
				return false;
			x += size;
			number = -number;
		}
		// Calculate divisor to extract digits from left to right
		int divisor = 1;
		int temp = number;
		while (temp >= 10) {
			temp /= 10;
			divisor *= 10;
		}
		// Draw
		while (divisor > 0) {
			int digit = number / divisor;
			if (!draw(renderer, (char)('0' + digit), x, y, size))
				return false;
			x += size;
			number %= divisor;
			divisor /= 10;
		}
		return true;
	}
};
