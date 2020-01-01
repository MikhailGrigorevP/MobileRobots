#pragma once
#ifndef RENDER_H
#define RENDER_H
#include "SDL.h"
#include <iostream>
#include "../Structures/structures.h"
#include <vector>
#include <map>
using std::map;

class render
{
private:

	SDL_Renderer* renderer = nullptr;
	SDL_Rect* _map_clip = nullptr;
	SDL_Rect* _map_source = nullptr;
	SDL_Texture* map_tex = nullptr;

public:

	SDL_Texture* robotScoutText = nullptr;
	SDL_Texture* robotScoutDeadText = nullptr;
	SDL_Texture* robotCommanderText = nullptr;
	SDL_Texture* ObserveCenterText = nullptr;
	SDL_Texture* ManagementCentrText = nullptr;

	SDL_Texture* fieldText = nullptr;
	SDL_Texture* fieldUpText = nullptr;
	SDL_Texture* fieldDwText = nullptr;
	SDL_Texture* fieldLText = nullptr;
	SDL_Texture* fieldRText = nullptr;
	SDL_Texture* borderText = nullptr;
	SDL_Texture* pointText = nullptr;
	SDL_Texture* barrierText = nullptr;

	SDL_Window* window = nullptr;
	render(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void render_map(vector<vector<unsigned>>& field, map<Point, Point> prev);
	~render() {};

	void add_texture_to_render(SDL_Texture* texture, SDL_Rect* _dst);

	void play();
	void init_texture();
	void init_text(vector<char const*> budget);
	void stop();

	void start();
	void updateFireIntensityPerPixel(int);
	void renderFire();
	void calculateFirePropagation();

};
#endif
