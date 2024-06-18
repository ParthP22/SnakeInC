#ifndef GAME_OBJECTS_H_
#define GAME_OBJECTS_H_


typedef struct Apple Apple;
typedef struct Snake Snake;

#include "main.h"
#include "game_state.h"
#include <time.h>

struct Apple{

	int x;
	int y;
	SDL_Texture* mc_apple;

};

struct Snake{

	int body_count;
	int apples_eaten;
	
	char direction;
	//bool rainbow;


	//These are x and y coords of each body part of the snake; there can be 576 total body parts; 
	//At the 0-position is the first body part
	int x[LENGTH * HEIGHT / (UNIT_SIZE * UNIT_SIZE)];
	int y[LENGTH * HEIGHT / (UNIT_SIZE * UNIT_SIZE)];

};



void init_objects(Game_State* game);
void move(Game_State* game);
void spawn_apple(Game_State* game);
void eat_apple(Game_State* game);
void collision_detection(Game_State* game);
void free_objects(Game_State* game);

#endif