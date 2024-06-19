#include "game_objects.h"


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
		snake->x[0] > LENGTH - 1 ||
		snake->y[0] < 0 + UNIT_SIZE * 3 ||
		snake->y[0] > HEIGHT + UNIT_SIZE * 3);
	eat_apple(game);
	return !(game->running);
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

