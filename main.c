#include "main.h"


int main(int argc, char** argv) {

	SDL_Window* window = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	window = SDL_CreateWindow("SnakeGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LENGTH,HEIGHT + UNIT_SIZE * 3,0);
	Game_State game;
	game.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_Event event;

	start_game(window,&game);

	bool done = false;
	//game.apple = NULL;
	//game.snake = NULL;
	//game.apple = (Apple*)malloc(sizeof(Apple));
	//game.snake = (Snake*)malloc(sizeof(Snake));
	//draw_grid(&game);
	
	//draw_grid(&game);
	while (!done) {
		done = start_screen(window,&game,&event);
	}
	done = false;
	while (!done && game.snake->direction != 'X') {
		/*
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);

					window = NULL;
					done = true;
				}
				break;
			case SDL_QUIT:
				done = true;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					done = true;
				}
				break;
			}
		}
		*/
		done = keyboard_control(window, &game, &event);
		if (game.running) {
			move(&game);
			render_frame(&game);

			collision_detection(&game);
		}
		else {
			game_over(&game);
		}
		
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyTexture(game.apple->mc_apple);
	if (game.score != NULL) {
		SDL_DestroyTexture(game.score);
	}
	TTF_CloseFont(game.font[0]);
	TTF_CloseFont(game.font[1]);
	SDL_Quit();
	TTF_Quit();
	free(game.snake);
	free(game.apple);
	free(game.label[0]);
	free(game.label[1]);
	free(game.label[2]);
	free(game.label[3]);

	return 0;
}