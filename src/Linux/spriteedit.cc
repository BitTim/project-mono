#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "../../lib/var.hh"
#include "../../lib/datatypes.hh"
#include "../../lib/gui.hh"
#include "../../lib/tcs_file.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

Spritesheet guiSprites;

//================================
// onClick Function templates
//================================

void newSprite();
void newSpriteshet();
void openSpritesheet();
void saveSpritesheet();
void saveSpritesheetAs();
void close();

void removeSprite();
void changePalette();
void 

//================================
// Utility Functions
//================================

void init()
{
    //Override _SCREENRES from var.hh
    _SCREENRES[0] = 640;
    _SCREENRES[1] = 480;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Tetra Spritesheet editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES[0], _SCREENRES[1], 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    //Load GUI Sprites
    if(guiSprites.load_file("dat/gui_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/gui_sprites.tcs\"\n");
		exit(-1);
	}

    //Clear the Window
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void end()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//================================
// onClick Functions
//================================



//================================
// Repeating Functions
//================================

void update()
{
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT) quit = true;
}

void draw()
{
    SDL_RenderPresent(renderer);
}

//================================
// Main
//================================

int main()
{
    init();

    while(!quit)
    {
        update();
        draw();
    }

    end();
}