#include <iostream>
#include <string>
#include <vector>

#include "var.hh"
#include "datatypes.hh"

SDL_Window* window;
SDL_Rendrer* renderer;
SDL_Event event;
bool quit = false;

//================================
// onClick function templates
//================================


//================================
// Utility functions
//================================

void init()
{
  _SCREENRES = Vec2(640, 480);

  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("Tetra Sprite Editor", SDL_WINDOWSPOS_UNDEFINED, SDL_WINDOWSPOS_UNDEFINED, _SCREENRES.x, _SCREENRES.y, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  //Load GUI Sprites
    if(guiSprites.loadFile("dat/gui_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/gui_sprites.tcs\"\n");
		exit(-1);
	}

  //Load Palettes
	if(guiPalettes.loadFile("dat/guipalettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/guiPalettes.tcp\"\n");
		exit(-1);
	}
}

void end()
{

}

//================================
// Repeated functions
//================================

void update()
{

}

void draw()
{

}

//================================
// onClick functions
//================================

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
  return 0;
}