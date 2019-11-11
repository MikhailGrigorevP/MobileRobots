#include "render.h"



void render::init_texture() {

	//SDL_Surface* WalkSurface = SDL_LoadBMP("Texture/sample/walk.bmp");
	//tex.Walk = SDL_CreateTextureFromSurface(renderer, WalkSurface);
	//SDL_FreeSurface(WalkSurface);
	//SDL_SetTextureBlendMode(tex.Walk, SDL_BLENDMODE_BLEND);
	//
	//SDL_Surface* Walk_BackSurface = SDL_LoadBMP("Texture/sample/walk_back.bmp");
	//tex.Walk_Back = SDL_CreateTextureFromSurface(renderer, Walk_BackSurface);
	//SDL_FreeSurface(Walk_BackSurface);
	//SDL_SetTextureBlendMode(tex.Walk_Back, SDL_BLENDMODE_BLEND);

}

render::render(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = 0;
	if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Init comlete" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
		if (renderer) {
			std::cout << "Renderer created" << std::endl;
		}
		int w, h;
		SDL_GetRendererOutputSize(renderer, &w, &h);

		_map_clip = new SDL_Rect;
		_map_source = new SDL_Rect;
		_map_clip->x = _map_source->x = 0;
		_map_clip->y = _map_source->y = 0;
		_map_clip->w = _map_source->w = w;
		_map_clip->h = _map_source->h = h;

		init_texture();
	}
}
