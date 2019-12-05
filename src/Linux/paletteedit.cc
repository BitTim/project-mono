#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <algorithm>

#include "../../lib/var.hh"
#include "../../lib/datatypes.hh"
#include "../../lib/gui.hh"
#include "../../lib/tcs_file.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;
bool mousePressed = false;
Vec2 mousePos;

Spritesheet guiSprites;
Spritesheet pSprites;
Palettelist palettelist;
Palettelist cPalettelist;
Palette cPalette;
int cPaletteID = 0;

MenuBar menubar;
TextBox colorVals[16];
Label colorValLabels[16];

//================================
// onClick Function templates
//================================

void newPalettelist();
void newPalette();
void openPalettelist();
void savePalettelist();
void savePalettelistAs();

void removePalette();

void about();

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

    if(pSprites.load_file("dat/player_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/player_sprites.tcs\"\n");
		exit(-1);
	}

    //Load Palettes
	if(palettelist.load_file("dat/palettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/palettes.tcp\"\n");
		exit(-1);
	}

    //Clear the Window
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //Initialize GUI
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(y == 0) colorValLabels[y * 4 + x] = Label("R", Vec2(2 * 16 + x * 5 * 16, 4 * 16 + y * 2 * 16));
            if(y == 1) colorValLabels[y * 4 + x] = Label("G", Vec2(2 * 16 + x * 5 * 16, 4 * 16 + y * 2 * 16));
            if(y == 2) colorValLabels[y * 4 + x] = Label("B", Vec2(2 * 16 + x * 5 * 16, 4 * 16 + y * 2 * 16));
            if(y == 3) colorValLabels[y * 4 + x] = Label("A", Vec2(2 * 16 + x * 5 * 16, 4 * 16 + y * 2 * 16));

            colorVals[y * 4 + x] = TextBox(2, Vec2(4 * 16 + x * 5 * 16, 4 * 16 + y * 2 * 16), y == 3 ? "FF" : "00", 2);
        }
    }

    cPalettelist.palettes.push_back(cPalette);

    std::vector<std::vector<std::string>> entries = {{"FILE", "EDIT", "HELP"}, {"NEW PALETTE", "NEW PALETTELIST", "OPEN PALETTELIST", "SAVE PALETTELIST", "SAVE PALETTELIST AS"}, {"REMOVE PALETTE"}, {"ABOUT"}};
    std::vector<std::vector<onClickFunc>> functions = {{newPalette, newPalettelist, openPalettelist, savePalettelist, savePalettelistAs}, {removePalette}, {about}};
    menubar = MenuBar(entries, functions, 16);
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

void newPalette()
{
    for(int i = 0; i < 16; i++) colorVals[i].content = i > 11 ? "FF" : "00";
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color(stoi(colorVals[0 + i].content, 0, 16), stoi(colorVals[4 + i].content, 0, 16), stoi(colorVals[8 + i].content, 0, 16), stoi(colorVals[12 + i].content, 0, 16));
    cPalettelist.palettes.push_back(cPalette);
    cPaletteID++;
}

void newPalettelist()
{
    cPalettelist.palettes.clear();
    for(int i = 0; i < 16; i++) colorVals[i].content = i > 11 ? "FF" : "00";
}

void openPalettelist()
{
    //Dialog with TextBox
}

void savePalettelist() { }
void savePalettelistAs() { }

void removePalette()
{
    if(cPalettelist.palettes.size() > 1)
    {
        cPaletteID -= 1;
        cPalettelist.palettes.pop_back();
        cPalette = cPalettelist.palettes[cPaletteID];
        for(int i = 0; i < 4; i++)
        {
            char tmpchr[3];

            sprintf(tmpchr, "%02x", cPalette.col[i].r);
            colorVals[i + 4 * 0].content = tmpchr;

            sprintf(tmpchr, "%02x", cPalette.col[i].g);
            colorVals[i + 4 * 1].content = tmpchr;

            sprintf(tmpchr, "%02x", cPalette.col[i].b);
            colorVals[i + 4 * 2].content = tmpchr;

            sprintf(tmpchr, "%02x", cPalette.col[i].a);
            colorVals[i + 4 * 3].content = tmpchr;
        }
    }
}

void about() { }

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

    for(int i = 0; i < 16; i++) colorVals[i].inHandle(event, mousePressed, mousePos);
    menubar.inHandle(mousePos, mousePressed);

    for(int i = 0; i < 16; i++) std::replace(colorVals[i].content.begin(), colorVals[i].content.end(), '\0', '0');
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color(stoi(colorVals[0 + i].content, 0, 16), stoi(colorVals[4 + i].content, 0, 16), stoi(colorVals[8 + i].content, 0, 16), stoi(colorVals[12 + i].content, 0, 16));
    cPalettelist.palettes[cPaletteID] = cPalette;
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
    SDL_RenderClear(renderer);

    //Draw GUI
    for(int i = 0; i < 16; i++) colorValLabels[i].draw(renderer, guiSprites, palettelist);
    for(int i = 0; i < 16; i++) colorVals[i].draw(renderer, guiSprites, palettelist);

    menubar.draw(renderer, guiSprites, palettelist);

    //Draw sample colors
    SDL_Rect tmp_rect;

    for(int i = 0; i < 4; i++)
    {
        iSDL_SetRenderDrawColor(renderer, cPalette.col[i]);
        tmp_rect = iSDL_Rect(2 * 16 + i * 5 * 16, 14 * 16, 4 * 16, 4 * 16);
        SDL_RenderFillRect(renderer, &tmp_rect);
    }

    //Draw Sample Sprite
    pSprites.drawSprite(renderer, 0, Vec2(23, 3), cPalettelist, cPaletteID, 16);

    //Draw Palletelist
    SDL_Rect 

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