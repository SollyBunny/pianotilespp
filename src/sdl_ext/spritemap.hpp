#pragma once

#include <SDL3/SDL.h>

#include "../lib/exception.hpp"

#include "loadimage.hpp"

class SDL_SpriteMap {
private:
	SDL_Texture* texture;

public:
	int tileW, tileH, tilesW, tilesH;
	SDL_Texture* const& getTexture() const {
		return texture;
	}
	SDL_SpriteMap(SDL_Renderer* const& renderer, const char* filename, int tilesWide, int tilesHigh, int tileWidth = 0, int tileHeight = 0)
		: tileW(tileWidth), tileH(tileHeight), tilesW(tilesWide), tilesH(tilesHigh)
	{
		texture = checkFunctionSDL(SDL_LoadImage, renderer, filename);
		float textureWidthF, textureHeightF;
		checkFunctionSDL(SDL_GetTextureSize, texture, &textureWidthF, &textureHeightF);
		int textureWidth = (int)textureWidthF, textureHeight = (int)textureHeightF;
		if (tileW <= 0)
			tileW = textureWidth / tilesW;
		if (textureWidth < tileWidth * tilesWide)
			raiseException("textureWidth < tileWidth * tilesWide: %d < %d * %d", textureWidth, tileWidth, tilesWide);
		if (tileH <= 0)
			tileH = textureHeight / tilesH;
		if (textureHeight < tileHeight * tilesHigh)
			raiseException("textureHeight < tileHeight * tilesHigh: %d < %d * %d", textureHeight, tileHeight, tilesHigh);
	}
	bool draw(SDL_Renderer* const& renderer, int tileX, int tileY, float x, float y, float w, float h) {
		if (tileX < 0 || tileX >= tilesW || tileY < 0 || tileY >= tilesH)
			return false;
		SDL_FRect src(tileX * tileW, tileY * tileH, tileW, tileH);
		SDL_FRect dst(x, y, w, h);
		return SDL_RenderTexture(renderer, texture, &src, &dst);
	}
	bool draw(SDL_Renderer* const& renderer, int tileX, int tileY, float x, float y, float w) {
		return draw(renderer, tileX, tileY, x, y, w, w * tileH / tileW);
	}
	bool draw(SDL_Renderer* const& renderer, int tileX, int tileY, float x, float y) {
		return draw(renderer, tileX, tileY, x, y, tileW, tileH);
	}
	~SDL_SpriteMap() {
		SDL_DestroyTexture(texture);
	}
};
