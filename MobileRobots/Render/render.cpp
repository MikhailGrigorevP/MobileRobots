#include "render.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL_ttf.h>

void render::add_texture_to_render(SDL_Texture* texture, SDL_Rect* _dst) {
	SDL_RenderCopy(renderer, texture, NULL, _dst);
}

typedef struct {
	SDL_Rect draw_rect;    // dimensions of button
	struct {
		Uint8 r, g, b, a;
	} colour;

	bool pressed;
} button_t;

static void button_process_event(button_t* btn, const SDL_Event* ev) {
	// react on mouse click within button rectangle by setting 'pressed'
	if (ev->type == SDL_MOUSEBUTTONDOWN) {
		if (ev->button.button == SDL_BUTTON_LEFT &&
			ev->button.x >= btn->draw_rect.x &&
			ev->button.x <= (btn->draw_rect.x + btn->draw_rect.w) &&
			ev->button.y >= btn->draw_rect.y &&
			ev->button.y <= (btn->draw_rect.y + btn->draw_rect.h)) {
			btn->pressed = true;
		}
	}
}

static bool button(SDL_Renderer* r, button_t* btn) {
	// draw button
	SDL_SetRenderDrawColor(r, btn->colour.r, btn->colour.g, btn->colour.b, btn->colour.a);
	SDL_RenderFillRect(r, &btn->draw_rect);

	// if button press detected - reset it so it wouldn't trigger twice
	if (btn->pressed) {
		btn->pressed = false;
		return true;
	}
	return false;
}

void render::test() {
	int quit = 0;

	SDL_Texture* txt = NULL;

	SDL_Rect rct;
	rct.x = 0;
	rct.y = 0;
	rct.h = 500;
	rct.w = 800;

	// button state - colour and rectangle
	button_t start_button;
	start_button.colour.r = 255;
	start_button.colour.g = 255;
	start_button.colour.b = 255;
	start_button.colour.a = 255;

	start_button.draw_rect = { 28, 128,128,  128 };

	enum states {
		STATE_IN_MENU,
		STATE_IN_GAME,
	} ;
	bool state = STATE_IN_MENU;
	while (!quit) {
		SDL_Event evt;    // no need for new/delete, stack is fine

		// event loop and draw loop are separate things, don't mix them
		while (SDL_PollEvent(&evt)) {
			// quit on close, window close, or 'escape' key hit
			if (evt.type == SDL_QUIT ||
				(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
				(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) {
				quit = 1;
			}

			// pass event to button
			button_process_event(&start_button, &evt);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//      SDL_RenderCopy(renderer, txt, NULL, &rct);

		if (state == STATE_IN_MENU) {
			if (button(renderer, &start_button)) {
				printf("start button pressed\n");
				state = STATE_IN_GAME;   // state change - button will not be drawn anymore
			}
		}
		else if (state == STATE_IN_GAME) {
			/* your game logic */
		}

		SDL_RenderPresent(renderer);
	}
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

	for (size_t i = 0; i < budget.size(); i++)
	{


		TTF_Font* font = TTF_OpenFont("ps.ttf", 200);

		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* surface = TTF_RenderText_Solid(font,
			budget[i], color);

		SDL_Rect r;

		r.x = 25;
		r.w = 40;
		r.h = 20;
		r.y = 20 + r.h*i;

	   
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

void render::stop() {

	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	TTF_Init();


		TTF_Font* font = TTF_OpenFont("ps.ttf", 300);

		SDL_Color color = { 255, 2, 2 };
		SDL_Surface* surface = TTF_RenderText_Solid(font,
			"Game over", color);

		SDL_Rect r;

		r.w = 200;
		r.h = 50;

		int x = (_map_source->w - r.w) / 2;
		int y = (_map_source->h - r.h) / 2;

		r.x = x;
		r.y = y;


		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &r);

		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
		TTF_CloseFont(font);
	

	TTF_Quit();
	SDL_Delay(10000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
