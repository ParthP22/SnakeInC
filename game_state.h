#ifndef GAME_STATE_H_
#define GAME_STATE_H_



typedef struct Game_State Game_State;

#include "game_objects.h"
#include "main.h"

struct Game_State{

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

};



// bool window_control(SDL_Window* window);
void draw_grid(Game_State* game);
void render_frame(Game_State* game);
bool keyboard_control(SDL_Window* window, Game_State* game, SDL_Event* event);
bool start_screen(SDL_Window* window, Game_State* game, SDL_Event* event);
void start_game(SDL_Window* window, Game_State* game);
bool game_over(Game_State* game);

#endif