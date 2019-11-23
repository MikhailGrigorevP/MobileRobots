#include "render.h"

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

void render::render_map(vector<vector<unsigned>>& field) {

	SDL_SetRenderTarget(renderer, map_tex);

	int map_room_w = 32;
	int map_room_h = 32;

	int size = field.size();
	int map_size_w = size * 32;
	int map_size_h = size * 32;

	int x = (_map_source->w - map_size_w) / 2;
	int y = (_map_source->h - map_size_h) / 2;

	//DRAW MAP
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

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
			case border:
				SDL_RenderCopy(renderer, borderText, NULL, &r);
				break;
			case barrier:
				SDL_RenderCopy(renderer, barrierText, NULL, &r);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &r);
				break;
			}



		}
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, map_tex, _map_clip, _map_source);

	SDL_RenderPresent(renderer);
}
