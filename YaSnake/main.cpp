
#include <iostream>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "map.h"
#include "snake.h"
#include "food.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& fn);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color ttf_color);

int main(int argc, char* argv[]) {

	Map gameMap{};

	// Font initializations
	if (TTF_Init() == -1) {
		std::cout << "Init TTF  failt : " << SDL_GetError() << std::endl;
		return -1;
	}
	
	// SDL initializations
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"YaSnake",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		gameMap.getWindowWidth(),
		gameMap.getWindowHeight(),
		0
	);
	if (window == nullptr) {
		std::cerr << "Failed to create SDL_window: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	if (renderer == nullptr) {
		std::cerr << "Failed to create SDL_renderer: " << SDL_GetError() << std::endl;
		return -1;
	}

	TTF_Font* font = TTF_OpenFont("./Amatic-Bold.ttf", 60);
	TTF_Font* font_small = TTF_OpenFont("./Amatic-Bold.ttf", 36);

	SDL_Texture* wall_tex = loadTexture(renderer, "./tile1.png");

	SDL_Texture* snake_tex_head = loadTexture(renderer, "./snek_head.png");
	
	SDL_Texture* snake_tex_body = loadTexture(renderer, "./snek.png");

	SDL_Texture* crash_tex = loadTexture(renderer, "./crash.png");

	Snake snake{};
	Food food{};
	food.setFoodPos(renderer);

	// game start screen
	SDL_Event event;
	SDL_Color font_col = { 255, 0, 0, SDL_ALPHA_OPAQUE };
	bool start_game = false;
	while (!start_game) {
		while (SDL_PollEvent(&event)) {
			if (SDL_KEYDOWN == event.type) {
				if (SDLK_SPACE == event.key.keysym.sym) start_game = true;
			}
			SDL_SetRenderDrawColor(renderer, 128, 192, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);	// clear back buffer

			drawText(renderer, font, "yet another ...", 200, 50, font_col);
			drawText(renderer, font, "SNAKE GAME", 230, 130, font_col);
			drawText(renderer, font_small, "use  'W','A','S','D'   or   arrow keys to move", 130, 260, font_col);
			drawText(renderer, font_small, "press 'P' to toggle  pause  and  ESC to exit", 130, 310, font_col);
			drawText(renderer, font_small, "'SPACE' to start playing :)", 230, 400, font_col);

			SDL_RenderPresent(renderer);
		}
	}


	double previous_time = SDL_GetTicks();
	constexpr int fps = 140;

	while(snake.isRunning()){

		double new_time = SDL_GetTicks();
		double dt = (new_time - previous_time) / 1000.0;
		previous_time = new_time;
		if (dt == 0) {
			dt = 1;
		}

		snake.handleInput();
		if (snake.checkFoodCollison(food)) {
			food.setFoodPos(renderer);
		}
		snake.updatePosition(gameMap.getWindowWidth(), gameMap.getWindowHeight());

		SDL_SetRenderDrawColor(renderer, 128, 192, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);	// clear back buffer

		SDL_Rect foodRect{ food.getPosX(), food.getPosY(), TILE_SIZE, TILE_SIZE };
		SDL_RenderCopy(renderer, food.getFoodTexture(), NULL, &foodRect);

		
		for (auto i = 0; i < MAP_WIDTH; ++i) {
			for (auto j = 0; j < MAP_HEIGHT; ++j) {
				if (map[i + j * MAP_WIDTH] == '0') {
					SDL_Rect rect{ static_cast<int>(i * TILE_SIZE),
								   static_cast<int>(j * TILE_SIZE), 
								   TILE_SIZE,
								   TILE_SIZE };
					SDL_RenderCopy(renderer, wall_tex, NULL, &rect);
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

		int k = snake.getSnakeSize();
		for (auto i = 0; i < k-1; i = i + TILE_SIZE / 8) {
			SDL_Rect dst_rect{ snake.getPosX(i), snake.getPosY(i), TILE_SIZE, TILE_SIZE };
			SDL_RenderCopy(renderer, snake_tex_body, NULL, &dst_rect);
		}

		SDL_Rect dst_rect{ snake.getPosX(k-1), snake.getPosY(k-1), TILE_SIZE, TILE_SIZE };
		if (snake.isDead()) {
			SDL_RenderCopy(renderer, crash_tex, NULL, &dst_rect);
			drawText(renderer, font, "GAME OVER", 230, 130, font_col);
			drawText(renderer, font, "score:", 230, 200, font_col);
			std::stringstream ss;
			ss << snake.getScore();
			std::string score = ss.str();
			drawText(renderer, font, score, 330, 200, { 100,0,0,SDL_ALPHA_OPAQUE });
		}
		else {
			SDL_RenderCopy(renderer, snake_tex_head, NULL, &dst_rect);
		}


		SDL_RenderPresent(renderer);	// swap back and front buffer -- pushes one frame to the window

		if ((1000.0 / fps) > (SDL_GetTicks() - previous_time)) {
			SDL_Delay(static_cast<Uint32>((1000.0 / static_cast<double>(fps)) - (SDL_GetTicks() - previous_time)));
		}
	}

	SDL_DestroyTexture(wall_tex);
	SDL_DestroyTexture(snake_tex_head);
	SDL_DestroyTexture(snake_tex_body);
	SDL_DestroyTexture(crash_tex);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& fn) {
	SDL_Texture* tmp = IMG_LoadTexture(renderer, fn.c_str());
	if (tmp == nullptr) {
		std::cout << "Failed to load texture " << fn << " error : " << SDL_GetError() << std::endl;
		return nullptr;
	}
	else {
		return tmp;
	}
}

void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color ttf_color) {
	SDL_Surface* font_surface;
	SDL_Texture* font_tex;

	font_surface = TTF_RenderText_Solid(font, text.c_str(), ttf_color);
	font_tex = SDL_CreateTextureFromSurface(renderer, font_surface);

	int w, h;
	SDL_QueryTexture(font_tex, NULL, NULL, &w, &h);

	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;

	SDL_RenderCopy(renderer, font_tex, NULL, &pos);

	SDL_DestroyTexture(font_tex);
	SDL_FreeSurface(font_surface);

}