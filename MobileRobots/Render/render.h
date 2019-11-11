#pragma once
#ifndef RENDER_H
#define RENDER_H
#include "SDL.h"
#include <iostream>

class render
{

private:

	SDL_Renderer* renderer = nullptr;
	SDL_Rect* _map_clip = nullptr;
	SDL_Rect* _map_source = nullptr;

public:

	SDL_Texture* robotScoutText = nullptr;
	SDL_Texture* robotCommanderText = nullptr;
	SDL_Texture* ObserveCenterText = nullptr;
	SDL_Texture* ManagementCentrText = nullptr;

	SDL_Texture* fieldText = nullptr;
	SDL_Texture* borderText = nullptr;
	SDL_Texture* pointText = nullptr;

	SDL_Window* window = nullptr;
	render(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	~render() {};

	void init_texture();

};
#endif

