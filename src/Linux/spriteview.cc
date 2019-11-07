#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include "../../lib/Linux/tcp_file.hh"
#include "../../lib/Linux/tcs_file.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

Palettelist palettelist;
std::ifstream infile;

int cColor = 0;
int data[16][16];

Vec2 mousepos;
Vec2 mouseposRaster;
bool mousePressed = false;

//================================
// Utiliy Functions
//================================

void init(const char* path)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("Tetra Sprite Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 480, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  //Load Palettes
	if(palettelist.load_file("dat/palettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/palettes.tcp\"\n");
		exit(-1);
	}

  //Open input file
  char fullpath[100];
  sprintf(fullpath, "src-dat/%s.hex", path);
  infile.open(fullpath, std::ofstream::binary);

  //Prepare the Window
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
  SDL_RenderClear(renderer);

  //Draw the sprite
  Sprite sprite;
  char tmp_dword[4];

  for(int i = 0; i < 16; i++)
	{
	  infile.read(tmp_dword, 4);
	  sprite.data[i] = (((tmp_dword[0] << 24) & 0xFF000000) + ((tmp_dword[1] << 16) & 0x00FF0000) + ((tmp_dword[2] << 8) & 0x0000FF00) + (tmp_dword[3] & 0x000000FF));
	}

  for(int y = 0; y < 16; y++)
  {
		for(int x = 0; x < 16; x++)
    {
			iSDL_SetRenderDrawColor(renderer, palettelist.palettes[0].col[(sprite.data[y] >> (30 - (x * 2))) & 0x03]);
			SDL_Rect pixel = iSDL_Rect(x * 480 / 16, y * 480 / 16, 480 / 16, 480 / 16);
			SDL_RenderFillRect(renderer, &pixel);
		}
	}

  SDL_RenderPresent(renderer);
}

void end()
{
  infile.close();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

//================================
// Program Functions
//================================

void update()
{
  SDL_WaitEvent(&event);
  if(event.type == SDL_QUIT) quit = true;
}

//================================
// Main
//================================

int main(int argc, char* argv[])
{
  if(argc <= 1)
  {
    printf("[F] Error 101: You have to specify a filename!\n\n");
    printf("Example usage: ./bin/Linux/spriteedit testfile\n");
    printf("The file will be in src-dat/<FILENAME>.hex\n");
    exit(-1);
  }

  init(argv[1]);

  while(!quit)
  {
    update();
  }

  end();
  return 0;
}