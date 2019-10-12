#include <iostream>
#include <SDL2/SDL.h>

#include "../../lib/Linux/mos_file.hh"
#include "../../lib/Linux/mom_file.hh"
#include "../../lib/Linux/datatypes.hh"
#include "../../lib/Linux/var.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

Spritesheet tSprites;
Spritesheet pSprites;
mono_palette standard_palette = mono_palette(0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF);

GameMap cMap;
vec2f playerPosRaster(4.5f, 4.5f);
vec2f playerOffset(0.0f, 0.0f);

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
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT)
	{
		quit = true;
		return;
	}
}

void draw()
{
	playerOffset = cMap.draw_map(renderer, playerPosRaster, tSprites, standard_palette);
	printf("OFFX: %f   OFFY: %f\n", playerOffset.x, playerOffset.y);
	pSprites.draw_sprite(renderer, 0, gtop(vec2f(playerPosRaster.x - playerOffset.x, playerPosRaster.y - playerOffset.y)), standard_palette);
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
		update();
		draw();
	}

	exit();
	return 0;
}
