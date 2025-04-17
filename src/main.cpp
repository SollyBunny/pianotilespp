#include <exception>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>

#include <mutex>
#include <memory>

#include "app.hpp"

std::mutex appState;
std::unique_ptr<App> app = nullptr;

SDL_AppResult SDL_AppInit(void** _, int argc, char* argv[]) {
	std::scoped_lock lock(appState);
	if (!SDL_Init(0)) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	try {
		app = std::make_unique<App>(argc, argv);
	} catch (const std::exception& e) {
		app = nullptr;
		printf("App failed: %s\n", e.what());
		return SDL_APP_FAILURE;
	}
	if (!app->running)
		return SDL_APP_FAILURE;
	app->lastTime = SDL_GetTicksNS();
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* _, SDL_AppResult result) {
	(void)result;
	std::scoped_lock lock(appState);
	app = nullptr;
}

SDL_AppResult SDL_AppIterate(void* _) {
	const uint64_t timeBefore = SDL_GetTicksNS();
	std::scoped_lock lock(appState);
	if (!app || !app->running)
		return SDL_APP_FAILURE;
	app->frame();
	if(!app->running)
		return SDL_APP_FAILURE;
	const uint64_t timeAfter = SDL_GetTicksNS();
	uint64_t delta = timeAfter - timeBefore;
	const uint64_t deltaTarget = (uint64_t)(1.0e+9) / app->targetFps;
	const uint64_t deltaMin = 1; // A billion fps
	const uint64_t deltaMax = deltaTarget * 100; // 100x less than targetFps
	if (delta > deltaMax)
		delta = deltaMax;
	else if (delta < deltaMin)
		delta = deltaMin;
	app->fps = 1.0e+9 / (float)(timeAfter - app->lastTime);
	app->delta = 1.0f / app->fps;
	app->lastTime = timeAfter;
	SDL_DelayNS(deltaTarget - delta);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* _, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;
	std::scoped_lock lock(appState);
	if (!app || !app->running)
		return SDL_APP_FAILURE;
	app->event(*event);
	if (!app->running)
		return SDL_APP_FAILURE;
	return SDL_APP_CONTINUE;
}
