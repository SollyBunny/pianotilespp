#pragma once

struct SDL_Texture;
struct SDL_Renderer;

[[nodiscard]] SDL_Texture* SDL_LoadImage(SDL_Renderer* const& renderer, const char* filename);
