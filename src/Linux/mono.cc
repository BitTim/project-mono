#include <iostream>
#include <SDL2/SDL.h>

#include "../../lib/Linux/mos_file.hh"
#include "../../lib/Linux/mom_file.hh"
#include "../../lib/Linux/mot_file.hh"
#include "../../lib/Linux/player.hh"
#include "../../lib/Linux/datatypes.hh"
#include "../../lib/Linux/var.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

const Uint8 *key_state = SDL_GetKeyboardState(NULL);
unsigned int last_time = 0, current_time = 0;

Spritesheet tSprites;
Spritesheet pSprites;
mono_palette standard_palette = mono_palette(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF);

GameMap cMap;
Tilelist tilelist;
Player player;
Vec2f playerDrawOffset(0.0f, 0.0f);

//================================
// Utility Funtions
//================================

void init()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Mono", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES[0], _SCREENRES[1], _FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	//Load Spritesheets
	if(tSprites.load_file("dat/tile_sprites.mos") == -1)
	{
		printf("[F] Error 201: Failed to load spritesheet \"dat/tile_sprites.mos\"\n");
		quit = true;
	}

	if(pSprites.load_file("dat/player_sprites.mos") == -1)
	{
		printf("[F] Error 201: Failed to load spritesheet \"dat/player_sprites.mos\"\n");
		quit = true;
	}

	//Load Map
	if(cMap.load_file("dat/Maps/Test_01.mom") == -1)
	{
		printf("[F] Error 202: Failed to load map \"dat/Maps/Test_01.mom\"\n");
		quit = true;
	}

	//Load Tilelist
	if(tilelist.load_file("dat/tilelist.mot") == -1)
	{
		printf("[F] Error 203: Failed to load tilelist \"dat/tilelist.mot\"\n");
		quit = true;
	}

	//Init the player
	player = Player(Vec2f(3.0f, 1.0f), 0.01f);

	//Prepare the Window
	iSDL_SetRenderDrawColor(renderer, standard_palette.bg);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void exit()
{
	//Quit SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//================================
// Game Functions
//================================

void update()
{
	//Check SDL Events
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT)
	{
		quit = true;
		return;
	}

	//Check Button Input
	//Movement X
	if(key_state[SDL_SCANCODE_A] == 1) player.dir.x = -1;
	if(key_state[SDL_SCANCODE_D] == 1) player.dir.x = 1;
	if(key_state[SDL_SCANCODE_A] == 0 && key_state[SDL_SCANCODE_D] == 0) player.dir.x = 0;

	//Movement Y
	if(key_state[SDL_SCANCODE_W] == 1) player.dir.y = -1;
	if(key_state[SDL_SCANCODE_S] == 1) player.dir.y = 1;
	if(key_state[SDL_SCANCODE_W] == 0 && key_state[SDL_SCANCODE_S] == 0) player.dir.y = 0;

	//Update the player
	player.update(cMap, tilelist);
}

void draw()
{
	playerDrawOffset = cMap.draw_map(renderer, player.pos, tilelist, tSprites, standard_palette);
	player.draw_player(renderer, playerDrawOffset, pSprites, standard_palette);
	SDL_RenderPresent(renderer);
}

//================================
// Main Function
//================================

int main()
{
	init();

	while(!quit)
	{
		current_time = SDL_GetTicks();
		if(current_time >= last_time)
		{
			update();
			draw();
			last_time = current_time;
		}
	}

	exit();
	return 0;
}
