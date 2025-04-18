#include "menu.hpp"

#include "../app.hpp"

void ComponentMenu::frame() {
	SDL_FRect box = { 100.0f, 100.0f, 200.0f, 150.0f };
	SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
	SDL_RenderRect(app.renderer, &box); 
	SDL_RenderFillRect(app.renderer, &box);
}
