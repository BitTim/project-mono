#include <iostream>
#include <SDL2/SDL.h>

#include "../../lib/Linux/tcp_file.hh"
#include "../../lib/Linux/tcs_file.hh"
#include "../../lib/Linux/tcm_file.hh"
#include "../../lib/Linux/tct_file.hh"
#include "../../lib/Linux/player.hh"
#include "../../lib/Linux/gui.hh"
#include "../../lib/Linux/datatypes.hh"
#include "../../lib/Linux/var.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

const Uint8 *key_state = SDL_GetKeyboardState(NULL);
unsigned int last_time = 0, current_time = 0;

Palettelist palettelist;
word cPalette = 0;
Spritesheet tSprites;
Spritesheet pSprites;
Spritesheet guiSprites;
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
	window = SDL_CreateWindow("Tetra", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES[0], _SCREENRES[1], _FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	//Load Palettes
	if(palettelist.load_file("dat/palettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/palettes.tcp\"\n");
		exit(-1);
	}

	//Load Spritesheets
	if(tSprites.load_file("dat/tile_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/tile_sprites.tcs\"\n");
		exit(-1);
	}

	if(pSprites.load_file("dat/player_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/player_sprites.tcs\"\n");
		exit(-1);
	}

	if(guiSprites.load_file("dat/gui_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/gui_sprites.tcs\"\n");
		exit(-1);
	}

	//Load Map
	if(cMap.load_file("dat/Maps/Test_01.tcm") == -1)
	{
		printf("[F] Error 203: Failed to load map \"dat/Maps/Test_01.tcm\"\n");
		exit(-1);
	}

	//Load Tilelist
	if(tilelist.load_file("dat/tilelist.tct") == -1)
	{
		printf("[F] Error 204: Failed to load tilelist \"dat/tilelist.tct\"\n");
		exit(-1);
	}

	//Init the player
	player = Player(Vec2f(3.0f, 1.0f), 0.05f);

	//Prepare the Window
	iSDL_SetRenderDrawColor(renderer, palettelist.palettes[cPalette].col[0]);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void end()
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
	playerDrawOffset = cMap.draw_map(renderer, player.pos, tilelist, tSprites, palettelist, cPalette);
	player.draw_player(renderer, playerDrawOffset, pSprites, palettelist, cPalette);
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

	end();
	return 0;
}
