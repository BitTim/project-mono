#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include "../../lib/tcs_file.hh"
#include "../../lib/tcp_file.hh"
#include "../../lib/gui.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

Spritesheet guiSprites;
Palettelist palettelist;
std::ofstream outfile;

MenuBar menubar;

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
  window = SDL_CreateWindow("Tetra Sprite Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 484, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  //Load Gui Sprites
  if(guiSprites.load_file("dat/gui_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/gui_sprites.tcs\"\n");
		exit(-1);
	}

  //Load Palettes
	if(palettelist.load_file("dat/palettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/palettes.tcp\"\n");
		exit(-1);
	}

  //Open output file
  char fullpath[100];
  sprintf(fullpath, "src-dat/%s.hex", path);
  outfile.open(fullpath, std::ofstream::binary);
  printf("PATH: %s\n", fullpath);

  //Prepare the Window
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  //Prepare GUI
  std::vector<std::string> entries = {"FILE", "EDIT", "HELP"};
  menubar = MenuBar(entries, 20);
}

void end()
{
  outfile.close();

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
  if(event.type == SDL_MOUSEMOTION)
  {
    mousepos.x = event.motion.x;
    mousepos.y = event.motion.y;

    if(mousepos.x >= 20 && mousepos.x <= 440 && mousepos.y >= 20 && mousepos.y <= 440)
    {
       mouseposRaster = mousepos;

      mouseposRaster.x -= 20;
      mouseposRaster.y -= 20;

      mouseposRaster.x /= 440 / 16;
      mouseposRaster.y /= 440 / 16;
    }
    else mouseposRaster = Vec2(-1, -1);
  }

  if(event.type == SDL_MOUSEBUTTONDOWN)
  {
    if(event.button.button == SDL_BUTTON_LEFT) mousePressed = true;
  }

  if(event.type == SDL_MOUSEBUTTONUP)
  {
    if(event.button.button == SDL_BUTTON_LEFT) mousePressed = false;
  }

  if(mouseposRaster.x >= 0 && mouseposRaster.y >= 0 && mousePressed)
  {
    data[mouseposRaster.x][mouseposRaster.y] = cColor;
  }

  if(event.type == SDL_KEYDOWN)
  {
    if(event.key.keysym.sym == '1') cColor = 0;
    if(event.key.keysym.sym == '2') cColor = 1;
    if(event.key.keysym.sym == '3') cColor = 2;
    if(event.key.keysym.sym == '4') cColor = 3;

    if(event.key.keysym.sym == 'x')
    {
      dword exported[16];
      char dataBlock[64];

      for(int y = 0; y < 16; y++)
      {
        exported[y] = 0;

        for(int x = 0; x < 16; x++)
        {
          exported[y] += (data[x][y] & 0x03) << (30 - (x * 2));
        }
      }

      for(int i = 0; i < 64; i++)
      {
        dataBlock[i] = (exported[i / 4] >> ((3 - i % 4) * 8)) & 0xFF;
      }

      outfile.write(dataBlock, 64);

      printf("Written data: \n");
      for(int i = 0; i < 64; i++) printf("%02x ", dataBlock[i]);
      printf("\n");
    }
  }
}

void draw()
{
  //Clear Screen
  SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
  SDL_RenderClear(renderer);

  //Draw MenuBar
  menubar.draw(renderer, -1, guiSprites, palettelist);

  //Draw the canvas
  int w = 440 / 16;
  for(int y = 0; y < 16; y++)
  {
    for(int x = 0; x < 16; x++)
    {
      iSDL_SetRenderDrawColor(renderer, palettelist.palettes[0].col[data[x][y]]);
      SDL_Rect pixel = iSDL_Rect(20 + x * w, 20 + y * w, w, w);
      SDL_RenderFillRect(renderer, &pixel);
    }
  }

  //Draw the mouse Cursor
  if(mouseposRaster.x >= 0 && mouseposRaster.y >= 0)
  {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
    SDL_Rect cursor = iSDL_Rect(20 + mouseposRaster.x * w, 20 + mouseposRaster.y * w, w, w);
    SDL_RenderDrawRect(renderer, &cursor);
  }

  //Draw the Tiled Preview
  int pW = 144 / 3 / 16;
  for(int n = 0; n < 3; n++)
  {
    for(int m = 0; m < 3; m++)
    {
      for(int y = 0; y < 16; y++)
      {
        for(int x = 0; x < 16; x++)
        {
          iSDL_SetRenderDrawColor(renderer, palettelist.palettes[0].col[data[x][y]]);
          SDL_Rect pPixel = iSDL_Rect(480 + m * pW * 16 + x * pW, 320 + n * pW * 16 + y * pW, pW, pW);
          SDL_RenderFillRect(renderer, &pPixel);
        }
      }
    }
  }

  //Draw the palette
  for(int i = 0; i < 4; i++)
  {
    iSDL_SetRenderDrawColor(renderer, palettelist.palettes[0].col[i]);
    SDL_Rect color = iSDL_Rect(480, 20 + i * 70, 144, 70);
    SDL_RenderFillRect(renderer, &color);
  }

  //Draw the cursor for color
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
  SDL_Rect colorCursor = iSDL_Rect(480, 20 + cColor * 70, 144, 70);
  SDL_RenderDrawRect(renderer, &colorCursor);

  SDL_RenderPresent(renderer);
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
    printf("The resulting file will be in src-dat/<FILENAME>.hex\n");
    exit(-1);
  }

  init(argv[1]);

  while(!quit)
  {
    draw();
    update();
  }

  end();
  return 0;
}