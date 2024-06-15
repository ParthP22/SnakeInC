#include "main.h"

typedef struct {

	int x;
	int y;
	SDL_Texture* mc_apple;

} Apple;

typedef struct {

	int body_count;
	int apples_eaten;
	
	char direction;
	//bool rainbow;

	int x[LENGTH * HEIGHT / (UNIT_SIZE * UNIT_SIZE)];
	int y[LENGTH * HEIGHT / (UNIT_SIZE * UNIT_SIZE)];

} Snake;


typedef struct {

	Apple* apple;
	Snake* snake;
	bool rainbow;

	bool running;

	SDL_Renderer* renderer;

	TTF_Font* font[2];
	SDL_Texture* score;
	SDL_Texture* play;
	SDL_Texture* color[2];

	int colorWH[2][2];
	int playW;
	int playH;
	int scoreW;
	int scoreH;
	char** label;

} Game_State;


void move(Game_State* game) {
	for (int i = game->snake->body_count; i > 0; i--) {
		game->snake->x[i] = game->snake->x[i - 1];
		game->snake->y[i] = game->snake->y[i - 1];
		/*
		game->snake.x[i] = game->snake.x[i - 1];
		game->snake.y[i] = game->snake.y[i - 1];
		*/
	}

	switch (game->snake->direction) {
	case 'U':
		//game->snake.y[0] = game->snake.y[0] - UNIT_SIZE;
		game->snake->y[0] = game->snake->y[0] - UNIT_SIZE;
		break;
	case 'D':
		//game->snake.y[0] = game->snake.y[0] + UNIT_SIZE;
		game->snake->y[0] = game->snake->y[0] + UNIT_SIZE;
		break;
	case 'L':
		//game->snake.x[0] = game->snake.x[0] - UNIT_SIZE;
		game->snake->x[0] = game->snake->x[0] - UNIT_SIZE;
		break;
	case 'R':
		//game->snake.x[0] = game->snake.x[0] + UNIT_SIZE;
		game->snake->x[0] = game->snake->x[0] + UNIT_SIZE;
		break;
	}
}


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

void spawn_apple(Game_State* game) {
	Apple* apple = game->apple;


	for (int i = 0; i < game->snake->body_count; i++) {
		do {
			apple->x = (int)(rand() % (LENGTH / UNIT_SIZE)) * UNIT_SIZE;
			apple->y = (int)(rand() % (HEIGHT / UNIT_SIZE)) * UNIT_SIZE + UNIT_SIZE * 3;

		} while (apple->x == game->snake->x[i] && apple->y == game->snake->y[i]);
	}

	/*
	//Note to self: add a for loop to check that the new apple doesn't spawn under the 
	//body of the snake
	do {
		apple->x = (int)(rand() % (LENGTH / UNIT_SIZE)) * UNIT_SIZE; 
		apple->y = (int)(rand() % (HEIGHT / UNIT_SIZE)) * UNIT_SIZE + UNIT_SIZE * 3;

	} while (apple->x == game->snake->x[0] && apple->y == game->snake->y[0]);
	*/
}



void init_objects(Game_State* game) {

	
	game->apple = (Apple*)malloc(sizeof(Apple));
	game->snake = (Snake*)malloc(sizeof(Snake));

	game->snake->x[0] = 0;
	game->snake->y[0] = UNIT_SIZE * 3;

	
	game->snake->apples_eaten = 0;
	game->snake->body_count = 1;
	game->snake->direction = 'R';
	//game->snake->  = false;
	game->apple->x = 0;
	game->apple->y = 0;
	spawn_apple(game);
	game->running = true;
	

	game->scoreW = 0;
	game->scoreH = 0;
	game->playW = 0;
	game->playH = 0;
	game->label = (char **)malloc(sizeof(char *) * 7);
    if (game->label == NULL) {
        printf("Failed to allocate memory for game->label.\n");
        free(game);
        return;
    }

    game->label[0] = (char *)malloc(sizeof(char) * 15);
    game->label[1] = (char *)malloc(sizeof(char) * 15);
    game->label[2] = (char *)malloc(sizeof(char) * 15);
    game->label[3] = (char *)malloc(sizeof(char) * 30);
    game->label[4] = (char *)malloc(sizeof(char) * 35);
    game->label[5] = (char *)malloc(sizeof(char) * 45);
    game->label[6] = (char *)malloc(sizeof(char) * 45);

    // Check if all allocations succeeded
    for (int i = 0; i < 7; i++) {
        if (game->label[i] == NULL) {
            printf("Failed to allocate memory for game->label[%d].\n", i);
            for (int j = 0; j < i; j++) {
                free(game->label[j]);
            }
            free(game->label);
            free(game);
            return;
        }
    }

	if(strcpy_s(game->label[0], 15, "Score   ") != 0){
		printf("Failed strcpy_s on label[0] from \"Score \"");
	}
	if (strcpy_s(game->label[2], 15, "GAME   OVER") != 0) {
		printf("Failed strcpy_s on label[2] from \"GAME   OVER\"");
	}
	if (strcpy_s(game->label[3], 30, "PRESS   ENTER   TO   START") != 0) {
		printf("Failed strcpy_s on label[3] from \"PRESS   \"ENTER\"   TO   START\"");
	}
	if (strcpy_s(game->label[4], 35, "PRESS   ENTER   TO   PLAY   AGAIN") != 0) {
		printf("Failed strcpy_s on label[4] from \"PRESS   \"ENTER\"   TO   PLAY   AGAIN\"");
	}
	if (strcpy_s(game->label[5], 45, "PRESS   1   FOR   A   RAINBOW   SNAKE") != 0) {
		printf("Failed strcpy_s on label[4] from PRESS   R   FOR   A   RAINBOW   SNAKE");
	}
	if (strcpy_s(game->label[6], 45, "PRESS   2   FOR   A   GREEN   SNAKE") != 0) {
		printf("Failed strcpy_s on label[4] from PRESS   G   FOR   A   GREEN   SNAKE");
	}
	SDL_Surface* surface = NULL;

	surface = IMG_Load("mc_apple_png_file.png");
	if (surface == NULL) {
		printf("Cannot find mc_apple_png_file.png");
		SDL_Quit();
		exit(1);
	}
	game->apple->mc_apple = SDL_CreateTextureFromSurface(game->renderer,surface);
	SDL_FreeSurface(surface);

	game->font[0] = TTF_OpenFont("C:\\Users\\bapa7\\Documents\\C Stuff\\VSCodeProjects\\SnakeInC\\Fonts\\ARCADECLASSIC.ttf", 40);
	if (game->font[0] == NULL) {
		printf("Failed to open font file: %s", TTF_GetError());
	}

	game->font[1] = TTF_OpenFont("C:\\Users\\bapa7\\Documents\\C Stuff\\VSCodeProjects\\SnakeInC\\Fonts\\ARCADECLASSIC.ttf", 20);
	if (game->font[1] == NULL) {
		printf("Failed to open font file: %s", TTF_GetError());
	}

	
	

	/*
	game->apple.x = 0;
	game->apple.y = 0;
	game->snake.apples_eaten = 0;
	game->snake.body_count = 2;
	game->snake.direction = 'D';
	game->running = true;
	*/
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



void eat_apple(Game_State* game) {
	if (game->snake->x[0] == game->apple->x && game->snake->y[0] == game->apple->y) {
		game->snake->body_count++;
		game->snake->apples_eaten++;
		spawn_apple(game);
	}
}


bool collision_detection(Game_State* game) {
	Snake* snake = game->snake;
	for (int i = snake->body_count; i > 0; i--) {
		
		if ((snake->x[0] == snake->x[i]) && (snake->y[0] == snake->y[i])) {
			game->running = false;
			return !(game->running);
		}
	}
	//eat_apple(game);
	game->running = !(snake->x[0] < 0 ||
		snake->x[0] > LENGTH ||
		snake->y[0] < 0 + UNIT_SIZE * 3 ||
		snake->y[0] > HEIGHT + UNIT_SIZE * 3);
	eat_apple(game);
	return !(game->running);
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

void free_objects(Game_State* game) {
	free(game->snake);
	free(game->apple);
	free(game->label[0]);
	free(game->label[1]);
	free(game->label[2]);
	free(game->label[3]);
	free(game->label[4]);
	//free(game->label[5]);
	//free(game->label[6]);
	//free(game->apple->mc_apple);
	//TTF_CloseFont(game->font[0]);
	//TTF_CloseFont(game->font[1]);
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




int main(int argc, char** argv) {

	SDL_Window* window = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	window = SDL_CreateWindow("SnakeGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LENGTH,HEIGHT + UNIT_SIZE * 3,0);
	Game_State game;
	game.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_Event event;
	bool done = false;
	//game.apple = NULL;
	//game.snake = NULL;
	//game.apple = (Apple*)malloc(sizeof(Apple));
	//game.snake = (Snake*)malloc(sizeof(Snake));
	//draw_grid(&game);
	init_objects(&game);
	
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