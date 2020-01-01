#include "render.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL_ttf.h>

void render::add_texture_to_render(SDL_Texture* texture, SDL_Rect* _dst) {
	SDL_RenderCopy(renderer, texture, NULL, _dst);
}



void render::init_texture() {

	SDL_Surface* fieldText_surf = SDL_LoadBMP("Textures/cell.bmp");
	fieldText  = SDL_CreateTextureFromSurface(renderer, fieldText_surf);
	SDL_FreeSurface(fieldText_surf);
	SDL_SetTextureBlendMode(fieldText, SDL_BLENDMODE_BLEND);

	SDL_Surface* ObserveCenterText_surf = SDL_LoadBMP("Textures/oc.bmp");
	ObserveCenterText = SDL_CreateTextureFromSurface(renderer, ObserveCenterText_surf);
	SDL_FreeSurface(ObserveCenterText_surf);
	SDL_SetTextureBlendMode(ObserveCenterText, SDL_BLENDMODE_BLEND);

	SDL_Surface* ManagementCentrText_surf = SDL_LoadBMP("Textures/cc.bmp");
	ManagementCentrText = SDL_CreateTextureFromSurface(renderer, ManagementCentrText_surf);
	SDL_FreeSurface(ManagementCentrText_surf);
	SDL_SetTextureBlendMode(ManagementCentrText, SDL_BLENDMODE_BLEND);

	SDL_Surface* robotScoutText_surf = SDL_LoadBMP("Textures/rs.bmp");
	robotScoutText = SDL_CreateTextureFromSurface(renderer, robotScoutText_surf);
	SDL_FreeSurface(robotScoutText_surf);
	SDL_SetTextureBlendMode(robotScoutText, SDL_BLENDMODE_BLEND);

	SDL_Surface* robotScoutDeadText_surf = SDL_LoadBMP("Textures/rsd.bmp");
	robotScoutDeadText = SDL_CreateTextureFromSurface(renderer, robotScoutDeadText_surf);
	SDL_FreeSurface(robotScoutDeadText_surf);
	SDL_SetTextureBlendMode(robotScoutDeadText, SDL_BLENDMODE_BLEND);

	SDL_Surface* robotCommanderText_surf = SDL_LoadBMP("Textures/rc.bmp");
	robotCommanderText = SDL_CreateTextureFromSurface(renderer, robotCommanderText_surf);
	SDL_FreeSurface(robotCommanderText_surf);
	SDL_SetTextureBlendMode(robotCommanderText, SDL_BLENDMODE_BLEND);

	SDL_Surface* pointText_surf = SDL_LoadBMP("Textures/inp.bmp");
	pointText = SDL_CreateTextureFromSurface(renderer, pointText_surf);
	SDL_FreeSurface(pointText_surf);
	SDL_SetTextureBlendMode(pointText, SDL_BLENDMODE_BLEND);

	SDL_Surface* barrierText_surf = SDL_LoadBMP("Textures/bar.bmp");
	barrierText = SDL_CreateTextureFromSurface(renderer, barrierText_surf);
	SDL_FreeSurface(barrierText_surf);
	SDL_SetTextureBlendMode(barrierText, SDL_BLENDMODE_BLEND);

	SDL_Surface* borderText_surf = SDL_LoadBMP("Textures/border.bmp");
	borderText = SDL_CreateTextureFromSurface(renderer, borderText_surf);
	SDL_FreeSurface(borderText_surf);
	SDL_SetTextureBlendMode(borderText, SDL_BLENDMODE_BLEND);
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

#include <string>
#include <math.h>


#include <time.h>

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 946;

SDL_Window* window;
SDL_Renderer* renderer;

int fireSize = 16;
int firePixelsArray[50000000] = { 0 };
int numberOfPixels = 0;

int fireColorsPalette[37][3] = { {7, 7, 7}, {31, 7, 7}, {47, 15, 7}, {71, 15, 7}, {87, 23, 7}, {103, 31, 7}, {119, 31, 7}, {143, 39, 7}, {159, 47, 7}, {175, 63, 7}, {191, 71, 7}, {199, 71, 7}, {223, 79, 7}, {223, 87, 7}, {223, 87, 7}, {215, 95, 7}, {215, 95, 7}, {215, 103, 15}, {207, 111, 15}, {207, 119, 15}, {207, 127, 15}, {207, 135, 23}, {199, 135, 23}, {199, 143, 23}, {199, 151, 31}, {191, 159, 31}, {191, 159, 31}, {191, 167, 39}, {191, 167, 39}, {191, 175, 47}, {183, 175, 47}, {183, 183, 47}, {183, 183, 55}, {207, 207, 111}, {223, 223, 159}, {239, 239, 199}, {255, 255, 255} };

using std::string;

void render::init_text(vector<char const*> budget) {
	SDL_RenderClear(renderer);
	TTF_Init();



	char* _string12 = new char[12];
	strcpy_s(_string12, 12, "");
	budget.push_back(_string12);

	char* _string123 = new char[12];
	strcpy_s(_string123, 12, "Current:");
	budget.push_back(_string123);


	budget.push_back(budget[budget.size()-3]);

	int start_p = 0;
	if (budget.size() >= 40) {
		start_p = budget.size() - 40;
	}

	for (size_t i = 0 + start_p; i < budget.size(); i++)
	{


		TTF_Font* font = TTF_OpenFont("ps.ttf", 200);

		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* surface = TTF_RenderText_Solid(font,
			budget[i], color);

		SDL_Rect r;

		r.x = 25;
		r.w = 40;
		r.h = 20;
		r.y = 20 + r.h*(i- start_p);

	   
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &r);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
		TTF_CloseFont(font);
	}

	vector<char const*>::iterator iter = budget.begin();

	budget.erase(iter + (budget.size() - 1));
	budget.erase(iter + (budget.size() - 1));
	budget.erase(iter + (budget.size() - 1));





	TTF_Quit();
}

void render::render_map(vector<vector<unsigned>>& field, map<Point, Point> prev) {

	SDL_SetRenderTarget(renderer, map_tex);

	int map_room_w = 32;
	int map_room_h = 32;

	int size_x = field.size();
	int size_y = field[0].size();
	int map_size_w = size_x * 32;
	int map_size_h = size_y * 32;

	int x = (_map_source->w - map_size_w) / 2;
	int y = (_map_source->h - map_size_h) / 2;

		//DRAW MAP
		for (int i = 0; i < size_y; i++) {
			for (int j = 0; j < size_x; j++) {

				SDL_Rect r;

				r.x = x + map_room_w * j;
				r.y = y + map_room_h * i;
				r.w = map_room_w;
				r.h = map_room_h;

				switch (field[j][i]) {
				case ai_seen:
					SDL_RenderCopy(renderer, fieldText, NULL, &r);
					break;
				case rc:
					SDL_RenderCopy(renderer, robotCommanderText, NULL, &r);
					break;
				case rs:
					SDL_RenderCopy(renderer, robotScoutText, NULL, &r);
					break;
				case rsd:
					SDL_RenderCopy(renderer, robotScoutDeadText, NULL, &r);
					break;
				case oc:
					SDL_RenderCopy(renderer, ObserveCenterText, NULL, &r);
					break;
				case cc:
					SDL_RenderCopy(renderer, ManagementCentrText, NULL, &r);
					break;
				case interest_point:
					SDL_RenderCopy(renderer, pointText, NULL, &r);
					break;
				case barrier:
					SDL_RenderCopy(renderer, barrierText, NULL, &r);
					break;
				case border:
					SDL_RenderCopy(renderer, borderText, NULL, &r);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &r);
					break;
				}
			}
		}


		map<Point, Point>::iterator it = prev.begin();
		for (; it != prev.end(); it++)
		{
			SDL_Rect r;
			r.x = x + map_room_w * (*it).second.x;
			r.y = y + map_room_h * (*it).second.y;
			r.w = map_room_w;
			r.h = map_room_h;

			SDL_RenderCopy(renderer, fieldText, NULL, &r);
		}

		for (size_t k = 1; k < 10; k++)
		{
			SDL_SetRenderTarget(renderer, map_tex);
		it = prev.begin();
		for (; it != prev.end(); it++)
		{

			SDL_Rect r;
			r.x = x + map_room_w * (*it).first.x;
			r.y = y + map_room_h * (*it).first.y;
			r.w = map_room_w;
			r.h = map_room_h;

			if ((*it).second.x > (*it).first.x) 
				r.x += 32 / 10 * k;

			else if ((*it).second.x < (*it).first.x)
				r.x -= 32 / 10 * k;
			else if ((*it).second.y > (*it).first.y) 
				r.y += 32 / 10 * k;
			else if ((*it).second.y < (*it).first.y)
				r.y -= 32 / 10 * k;
			
			r.w = map_room_w;
			r.h = map_room_h;

			if (field[(*it).first.x][(*it).first.y] == rs)
				SDL_RenderCopy(renderer, robotScoutText, NULL, &r);
			else	if (field[(*it).first.x][(*it).first.y] == rc)
				SDL_RenderCopy(renderer, robotCommanderText, NULL, &r);


		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, map_tex, _map_clip, _map_source);

		SDL_RenderPresent(renderer);
	}

		SDL_SetRenderTarget(renderer, map_tex);
		//DRAW MAP
		for (int i = 0; i < size_y; i++) {
			for (int j = 0; j < size_x; j++) {

				SDL_Rect r;

				r.x = x + map_room_w * j;
				r.y = y + map_room_h * i;
				r.w = map_room_w;
				r.h = map_room_h;

				switch (field[j][i]) {
				case ai_seen:
					SDL_RenderCopy(renderer, fieldText, NULL, &r);
					break;
				case rc:
					SDL_RenderCopy(renderer, robotCommanderText, NULL, &r);
					break;
				case rs:
					SDL_RenderCopy(renderer, robotScoutText, NULL, &r);
					break;
				case rsd:
					SDL_RenderCopy(renderer, robotScoutDeadText, NULL, &r);
					break;
				case oc:
					SDL_RenderCopy(renderer, ObserveCenterText, NULL, &r);
					break;
				case cc:
					SDL_RenderCopy(renderer, ManagementCentrText, NULL, &r);
					break;
				case interest_point:
					SDL_RenderCopy(renderer, pointText, NULL, &r);
					break;
				case barrier:
					SDL_RenderCopy(renderer, barrierText, NULL, &r);
					break;
				case border:
					SDL_RenderCopy(renderer, borderText, NULL, &r);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &r);
					break;
				}



			}
		}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, map_tex, _map_clip, _map_source);

	SDL_RenderPresent(renderer);

}


void render::start() {
	numberOfPixels = (WINDOW_WIDTH / fireSize) * (WINDOW_HEIGHT / fireSize);
	for (int i = 0; i < numberOfPixels; i++) {
		firePixelsArray[i] = 36;
	}
}

void render::updateFireIntensityPerPixel(int currentPixelIndex) {
	int belowPixelIndex = currentPixelIndex + (WINDOW_WIDTH / fireSize);
	if (belowPixelIndex < numberOfPixels) {
		int decay = floor(rand() % 3);
		int belowPixelFireIntensity = firePixelsArray[belowPixelIndex];
		int newFireIntensity =
			belowPixelFireIntensity - decay >= 0 ? belowPixelFireIntensity - decay : 0;
		int pos = (currentPixelIndex - decay >= 0) ? currentPixelIndex - decay : 0;
		firePixelsArray[pos] = newFireIntensity;
	}
}

void render::renderFire() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for (int x = 0; x < WINDOW_WIDTH; x += fireSize) {
		for (int y = 0; y < WINDOW_HEIGHT; y += fireSize) {
			int p = x + ((WINDOW_WIDTH / fireSize) * y);
			int pixelIndex = p / fireSize;
			int fireIntensity = firePixelsArray[pixelIndex];
			int colorR = fireColorsPalette[fireIntensity][0];
			int colorG = fireColorsPalette[fireIntensity][1];
			int colorB = fireColorsPalette[fireIntensity][2];
			SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
			SDL_Rect rectToDraw = { x, y, x + fireSize, y + fireSize };
			SDL_RenderFillRect(renderer, &rectToDraw);
		}
	}
	SDL_RenderPresent(renderer);
}

void render::calculateFirePropagation() {
	renderFire();
	for (int i = 0; i < numberOfPixels; i++) {
		updateFireIntensityPerPixel(i);
	}
}


void render::play() {
	SDL_SetRenderTarget(renderer, NULL);
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("ps.ttf", 300);

	SDL_Color color = { 204, 255, 204 };
	SDL_Surface* surface = TTF_RenderText_Solid(font,
		"Start", color);

	SDL_Rect r;

	r.w = 180;
	r.h = 100;

	int x = (_map_source->w - r.w) / 2;
	int y = (_map_source->h- r.h) / 2;

	r.x = x;
	r.y = y;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &r);

	SDL_RenderPresent(renderer);
	bool working = true;
	while (working) {
		int x;
		int y;

			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				switch (e.type)
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_LEFT) {
						working = false;
						//SDL_GetMouseState(&x, &y);
					}
					break;
			}
	}

SDL_DestroyTexture(texture);
SDL_FreeSurface(surface);
TTF_CloseFont(font);
}

void render::stop() {

	srand(time(NULL));

	SDL_SetRenderTarget(renderer, NULL);
	start();

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("ps.ttf", 300);

	SDL_Color color = { 255, 2, 2 };
	SDL_Surface* surface = TTF_RenderText_Solid(font,
		"Game over", color);

	SDL_Rect r;

	r.w = 200;
	r.h = 50;

	int x = (_map_source->w - r.w) / 2;
	int y = 200;

	r.x = x;
	r.y = y;


	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	bool is_running = true;
	SDL_Event event;
	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				is_running = false;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN)
				if (event.button.button == SDL_BUTTON_LEFT) {
					is_running = false;
					//SDL_GetMouseState(&x, &y);
				}
		}

		calculateFirePropagation();

		SDL_RenderCopy(renderer, texture, NULL, &r);

		SDL_RenderPresent(renderer);
		SDL_Delay(50);

	}


	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);


	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

