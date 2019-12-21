#include <iostream>
#include <SDL2/SDL.h>

#include "../../lib/gui.hh"
#include "../../lib/tcp_file.hh"
#include "../../lib/tcs_file.hh"
#include "../../lib/var.hh"
#include "../../lib/datatypes.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

bool quit = false;

Palettelist palettelist;
Spritesheet pSprites;
Spritesheet guiSprites;

Label label1;
TextBox textbox1;
TextButton submit;
SpriteButton button2;
TextButton superbutton;
Label label2;

bool mousePressed;
Vec2 mousePos;

//================================
// Program Function Templates
//================================

void printText();
void showOverlay();
void hideOverlay();

//================================
// Utility Functions
//================================

void init()
{
  _SCREENRES = Vec2(640, 480);
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("GUI Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES.x, _SCREENRES.y, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  //Load Palettes
	if(palettelist.load_file("dat/palettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/palettes.tcp\"\n");
		exit(-1);
	}

  //Load Sprites
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

  label1 = Label("Enter Text ", Vec2(30, 30), 1, 16);
  textbox1 = TextBox(1, Vec2(30 + 11 * 16, 30), 0, 1, 10, 16);
  submit = TextButton(1, Vec2(30 + 11 * 16 + 11 * 16, 30), "Submit", printText, 1, 16);
  label2 = Label("\0", Vec2(30, 60), 1, 16);
  button2 = SpriteButton(1, Vec2(30 + 11 * 16 + 11 * 16 + 48), pSprites, 0, 0, 0, 1, showOverlay, 16);
  superbutton = TextButton(2, Vec2(30, 90), "Back", hideOverlay, 1, 16);

  superbutton.visible = false;
}

void end()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

//================================
// Repeating Functions
//================================

void update()
{
  SDL_WaitEvent(&event);
  if(event.type == SDL_QUIT) quit = true;
  if(event.type == SDL_MOUSEBUTTONDOWN) mousePressed = true;
  if(event.type == SDL_MOUSEBUTTONUP) mousePressed = false;

  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  textbox1.inHandle(event, mousePressed, mousePos);
  submit.inHandle(event, mousePressed, mousePos);
  button2.inHandle(event, mousePressed, mousePos);
  superbutton.inHandle(event, mousePressed, mousePos);
}

void draw()
{
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);

  label1.draw(renderer, guiSprites, palettelist);
  label2.draw(renderer, guiSprites, palettelist);

  textbox1.draw(renderer, guiSprites, palettelist);

  submit.draw(renderer, guiSprites, palettelist);
  button2.draw(renderer, palettelist);
  superbutton.draw(renderer, guiSprites, palettelist);

  SDL_RenderPresent(renderer);
}

//================================
// Program Functions
//================================

void printText()
{
  label2.content = textbox1.content;
}

void showOverlay()
{
  superbutton.visible = true;
}

void hideOverlay()
{
  superbutton.visible = false;
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
  return 0;
}