#include "game_state.h"



// bool window_control(SDL_Window* window) {
// 	SDL_Event event;
// 	if (SDL_PollEvent(&event)) {
// 		switch (event.type) {
// 		case SDL_QUIT:
// 			if (window) {
// 				SDL_DestroyWindow(window);
// 				window = NULL;
// 				return true;
// 			}
// 		case SDL_KEYDOWN:
// 			if (event.key.keysym.sym == SDLK_ESCAPE) {
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }

void draw_grid(Game_State* game) {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderDrawColor(game->renderer,128,128,128,255);
	//Drawing the grid
	SDL_RenderDrawLine(game->renderer, 0, 0 + UNIT_SIZE * 3, LENGTH, 0 + UNIT_SIZE * 3);
	for (int i = 1; i < HEIGHT / UNIT_SIZE; i++) {
		SDL_RenderDrawLine(game->renderer, i * UNIT_SIZE, UNIT_SIZE * 3, i * UNIT_SIZE, HEIGHT + UNIT_SIZE * 3);
		SDL_RenderDrawLine(game->renderer, 0, i * UNIT_SIZE + UNIT_SIZE * 3, LENGTH, i * UNIT_SIZE + UNIT_SIZE * 3);
	}
	SDL_Delay(60);
	SDL_RenderPresent(game->renderer);
}

void render_frame(Game_State* game) {
	draw_grid(game);
	//SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
	//SDL_RenderClear(game->renderer);

	SDL_Rect apple = {game->apple->x, game->apple->y, UNIT_SIZE, UNIT_SIZE};
	SDL_RenderCopy(game->renderer, game->apple->mc_apple, NULL, &apple);

	//For green body snake
	//SDL_SetRenderDrawColor(game->renderer,0,200,0,255);
	for (int i = 0; i < game->snake->body_count; i++) {
		SDL_SetRenderDrawColor(game->renderer, ((game->rainbow) ? (rand() % 255) : 0), ((game->rainbow) ? (rand() % 255) : 255), ((game->rainbow) ? (rand() % 255) : 0), 255);
		SDL_Rect rect = { game->snake->x[i], game->snake->y[i],UNIT_SIZE, UNIT_SIZE };
		SDL_RenderFillRect(game->renderer,&rect);
	}

	char tmpScoreTxt[30];
	if (_itoa_s(game->snake->apples_eaten, game->label[1], 15, 10) != 0) {
		printf("Failed itoa_s");
	}
	if (strcpy_s(tmpScoreTxt, 15, game->label[0]) != 0) {
		printf("Failed strcpy_s on tmpScoreTxt from label[0]");
	}

	if (strcat_s(tmpScoreTxt, 15, game->label[1]) != 0) {
		printf("Failed strcat_s on label[1");
	}
	SDL_Surface* surface = TTF_RenderText_Solid(game->font[0], tmpScoreTxt, (SDL_Color) { 255, 255, 255, 255 });
	if (surface == NULL) {
		printf("Failed to render text: %s", TTF_GetError());
	}

	game->score = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (game->score == NULL) {
		printf("Failed to create texture of score label: %s", SDL_GetError());
	}

	game->scoreW = surface->w;
	game->scoreH = surface->h;
	SDL_FreeSurface(surface);
	SDL_Rect score = { (LENGTH - game->scoreW) / 2, UNIT_SIZE, game->scoreW,game->scoreH };
	SDL_RenderCopy(game->renderer, game->score, NULL, &score);
	SDL_RenderPresent(game->renderer);

	/*
	char tmpScoreTxt[25];
	_itoa_s(game->snake->apples_eaten, game->label[1], sizeof(game->label[1]), 10);
	strcpy_s(tmpScoreTxt, sizeof(game->label[0]), game->label[0]);
	strcat_s(tmpScoreTxt, sizeof(game->label[1]), game->label[1]);
	SDL_Surface* surface = TTF_RenderText_Solid(game->font, tmpScoreTxt, (SDL_Color) { 255, 255, 255, 255 });
	if (surface == NULL) {
		printf("Failed to render text: %s", TTF_GetError());
	}

	game->score = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (game->score == NULL) {
		printf("Failed to create texture of score label: %s", SDL_GetError());
	}



	SDL_Rect score = { LENGTH / 2, UNIT_SIZE, game->scoreW,game->scoreH };
	SDL_RenderCopy(game->renderer, game->score, NULL, &score);
	SDL_RenderPresent(game->renderer);
	*/

	SDL_Delay(60);
	SDL_RenderPresent(game->renderer);
	
}

bool keyboard_control(SDL_Window* window, Game_State* game, SDL_Event* event) {
	while (SDL_PollEvent(event)) {
			switch ((*event).type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);

					window = NULL;
					return true;
				}
				break;
			case SDL_QUIT:
				return true;
				break;
			/*
			case SDL_KEYDOWN:
				if ((*event).key.keysym.sym == SDLK_ESCAPE) {
					return true;
				}
				if (((*event).key.keysym.sym == SDLK_RETURN || (*event).key.keysym.sym == SDLK_KP_ENTER) && !game->running) {
					free_objects(game);
					init_objects(game);
					//game->running = true;

					return false;
			*/
			}
			if (!(game->running) && (*event).type == SDL_KEYDOWN) {
				switch ((*event).key.keysym.sym) {
				case SDLK_RETURN:
					free_objects(game);
					init_objects(game);
					return false;
					break;
				case SDLK_KP_ENTER:
					free_objects(game);
					init_objects(game);
					return false;
					break;
				case SDLK_1:
					game->rainbow = true;
					break;
				case SDLK_2:
					game->rainbow = false;
					break;
				}
			}
		/*
		switch ((*event).type) {
		case SDL_KEYDOWN:
			switch ((*event).key.keysym.sym) {
			case SDLK_DOWN:
				if (game->snake->direction != 'U') {
					game->snake->direction = 'D';
				}
				break;
			case SDLK_UP:
				if (game->snake->direction != 'D') {
					game->snake->direction = 'U';
				}
				break;
			case SDLK_LEFT:
				if (game->snake->direction != 'R') {
					game->snake->direction = 'L';
				}
				break;
			case SDLK_RIGHT:
				if (game->snake->direction != 'L') {
					game->snake->direction = 'R';
				}
				break;
			}
			break;
		}
		*/
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if(game->running){
			if (state[SDL_SCANCODE_UP] && game->snake->direction != 'D') {
				game->snake->direction = 'U';
			}
			if (state[SDL_SCANCODE_DOWN] && game->snake->direction != 'U') {
				game->snake->direction = 'D';
			}
			if (state[SDL_SCANCODE_LEFT] && game->snake->direction != 'R') {
				game->snake->direction = 'L';
			}
			if (state[SDL_SCANCODE_RIGHT] && game->snake->direction != 'L') {
				game->snake->direction = 'R';
			}
			/*
			if (state[SDL_SCANCODE_1] && !game->running) {
				game->snake->rainbow = true;
			}
			if (state[SDL_SCANCODE_2] && !game->running) {
				game->snake->rainbow = false;
			}
			*/
		}

	}
	return false;
}

bool start_screen(SDL_Window* window, Game_State* game, SDL_Event* event) {
	while (SDL_PollEvent(event)) {
		switch ((*event).type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);

				window = NULL;
				game->running = false;
				game->snake->direction = 'X';
				return true;
			}
			break;
		case SDL_QUIT:
			game->running = false;
			game->snake->direction = 'X';
			return true;
			
			break;

		case SDL_KEYDOWN:
			/*
			if ((*event).key.keysym.sym == SDLK_RETURN || (*event).key.keysym.sym == SDLK_KP_ENTER) {
				return true;
			}
			*/
			switch ((*event).key.keysym.sym) {
			case SDLK_RETURN:
				return true;
				break;
			case SDLK_KP_ENTER:
				return true;
				break;
			case SDLK_1:
				game->rainbow = true;
				break;
			case SDLK_2:
				game->rainbow = false;
				break;
			}
			break;
		}
	}
	//char tmpScoreTxt[45];
	SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
	SDL_RenderClear(game->renderer);

	//if (strcpy_s(tmpScoreTxt, 30, game->label[3]) != 0) {
	//	printf("Failed strcpy_s on tmpScoreTxt from label[3]");
	//}
	SDL_Surface* surface = TTF_RenderText_Solid(game->font[0], game->label[3], (SDL_Color) { 255, 255, 255, 255 });
	if (surface == NULL) {
		printf("Failed to render start screen text1: %s", TTF_GetError());
	}

	game->score = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (game->score == NULL) {
		printf("Failed to create texture of score label on start screen: %s", SDL_GetError());
	}
	game->scoreW = surface->w;
	game->scoreH = surface->h;

	SDL_FreeSurface(surface);




	//if (strcpy_s(tmpScoreTxt, 40, game->label[5]) != 0) {
	//	printf("Failed strcpy_s on tmpScoreTxt from label[5]");
	//}
	
	surface = TTF_RenderText_Solid(game->font[1], game->label[5], (SDL_Color) { 255, 255, 255, 255 });
	if (surface == NULL) {
		printf("Failed to render start screen text2: %s", TTF_GetError());
	}
	game->color[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (game->color[0] == NULL) {
		printf("Failed to create texture of green color label on start screen1: %s", SDL_GetError());
	}
	//This is for the width and height of the label for green
	game->colorWH[0][0] = surface->w;
	game->colorWH[0][1] = surface->h;

	SDL_FreeSurface(surface);

	//if (strcpy_s(tmpScoreTxt, 40, game->label[6]) != 0) {
	//	printf("Failed strcpy_s on tmpScoreTxt from label[6]");
	//}

	surface = TTF_RenderText_Solid(game->font[1], game->label[6], (SDL_Color) { 255, 255, 255, 255 });
	if (surface == NULL) {
		printf("Failed to render start screen text3: %s", TTF_GetError());
	}
	game->color[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (game->color[1] == NULL) {
		printf("Failed to create texture of green color label on start screen2: %s", SDL_GetError());
	}

	//This is for the width and height of the label for rainbow
	game->colorWH[1][0] = surface->w;
	game->colorWH[1][1] = surface->h;

	SDL_FreeSurface(surface);

	SDL_Rect score = { (LENGTH - game->scoreW) / 2, (HEIGHT - game->scoreH) / 2, game->scoreW,game->scoreH };
	SDL_Rect green = { (LENGTH - game->colorWH[0][0]) / 2, (HEIGHT - game->colorWH[0][1] + game->scoreH * 2) / 2, game->colorWH[0][0],game->colorWH[0][1] };
	SDL_Rect rainbow = { (LENGTH - game->colorWH[1][0]) / 2, (HEIGHT - game->colorWH[1][1] + (game->colorWH[0][1] + game->scoreH) * 2) / 2, game->colorWH[1][0],game->colorWH[1][1] };


	SDL_RenderCopy(game->renderer, game->score, NULL, &score);
	SDL_RenderCopy(game->renderer, game->color[0], NULL, &green);
	SDL_RenderCopy(game->renderer, game->color[1], NULL, &rainbow);
	SDL_Delay(60);
	SDL_RenderPresent(game->renderer);
	
}

void start_game(SDL_Window* window, Game_State* game) {
	game->running = true;
	init_objects(game->snake,game->apple);

	draw_grid(window,game);

}

bool game_over(Game_State* game) {
	if (!(game->running)) {
		SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
		SDL_RenderClear(game->renderer);
		SDL_Surface* surface = TTF_RenderText_Solid(game->font[0], game->label[2], (SDL_Color) { 255, 255, 255, 255 });
		if (surface == NULL) {
			printf("Failed to render text: %s", TTF_GetError());
		}


		game->score = SDL_CreateTextureFromSurface(game->renderer, surface);
		if (game->score == NULL) {
			printf("Failed to create texture of score label: %s", SDL_GetError());
		}

		game->scoreW = surface->w;
		game->scoreH = surface->h;
		SDL_FreeSurface(surface);

		surface = TTF_RenderText_Solid(game->font[1], game->label[4], (SDL_Color) { 255, 255, 255, 255 });

		game->play = SDL_CreateTextureFromSurface(game->renderer, surface);
		if (game->play == NULL) {
			printf("Failed to create texture of score label on start screen: %s", SDL_GetError());
		}
		game->playW = surface->w;
		game->playH = surface->h;


		SDL_FreeSurface(surface);




		SDL_Rect score = { (LENGTH - game->scoreW) / 2, (HEIGHT - game->scoreH * 2) / 2, game->scoreW,game->scoreH};
		SDL_Rect play = { (LENGTH - game->playW) / 2, (HEIGHT - game->playH + game->scoreH * 2) / 2, game->playW,game->playH };
		SDL_Rect green = { (LENGTH - game->colorWH[0][0]) / 2, (HEIGHT - game->colorWH[0][1] + (game->scoreH + game->playH) * 2) / 2, game->colorWH[0][0],game->colorWH[0][1] };
		SDL_Rect rainbow = { (LENGTH - game->colorWH[1][0]) / 2, (HEIGHT - game->colorWH[1][1] + (game->colorWH[0][1] + game->scoreH + game->playH) * 2) / 2, game->colorWH[1][0],game->colorWH[1][1] };


		SDL_RenderCopy(game->renderer, game->score, NULL, &score);
		SDL_RenderCopy(game->renderer, game->play, NULL, &play);
		SDL_RenderCopy(game->renderer, game->color[0], NULL, &green);
		SDL_RenderCopy(game->renderer, game->color[1], NULL, &rainbow);
		SDL_RenderPresent(game->renderer);

	}
}