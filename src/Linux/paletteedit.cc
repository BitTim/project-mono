#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <algorithm>

#include "../../lib/gui.hh"
#include "../../lib/tcs_file.hh"
#include "../../lib/tcp_file.hh"
#include "../../lib/var.hh"
#include "../../lib/datatypes.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;
bool mousePressed = false;
Vec2 mousePos;

Spritesheet guiSprites;
Palettelist guiPalettes;
Palettelist spritePalettes;
Palettelist cPalettelist;
Palette cPalette;
int cPaletteID = 0;

TextBox colorVals[4];
Label colorValLabels[4];

Panel menubarBG;
TextButton menubar[3];
TextButton fileMenu[4];
TextButton editMenu[3];

Panel dialogBG;
Label dialogText;
TextButton dialogButtons[2];

//================================
// onClick Function templates
//================================

void showFileMenu();
void hideFileMenu();

void showEditMenu();
void hideEditMenu();

void newFile();
void openFile();
void saveFile();
void saveFileAs();

void newPalette();
void removePalette();
void clearPalette();

void about();

//================================
// Utility Functions
//================================

void init()
{
    //Override _SCREENRES from var.hh
    _SCREENRES.x = 640;
    _SCREENRES.y = 480;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Tetra Spritesheet editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES.x, _SCREENRES.y, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    //Load GUI Sprites
    if(guiSprites.loadFile("dat/gui_sprites.tcs") == -1)
	{
		printf("[F] Error 202: Failed to load spritesheet \"dat/gui_sprites.tcs\"\n");
		exit(-1);
	}

    //Load Palettes
	if(guiPalettes.loadFile("dat/guiPalettes.tcp") == -1)
	{
		printf("[F] Error 201: Failed to load palettelist \"dat/guiPalettes.tcp\"\n");
		exit(-1);
	}

    //Clear the Window
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //Initialize GUI
    for(int i = 0; i < 4; i++)
    {
        colorValLabels[i] = Label("Color " + std::to_string(i), Vec2(2 * 16 + i * 9 * 16 + 16 / 2, 4 * 16), 0);
        colorVals[i] = TextBox(1, Vec2(2 * 16 + i * 9 * 16, 6 * 16), 2, 0, 8, 16, "000000FF");
    }

    cPalettelist.palettes.push_back(cPalette);

    menubarBG = Panel(Vec2(0, 0), Vec2(_SCREENRES.x, 16), 2, 0);
    menubar[0] = TextButton(1, Vec2(0, 0), "File", showFileMenu, 0);
    menubar[1] = TextButton(1, Vec2(5 * 16, 0), "Edit", showEditMenu, 0);
    menubar[2] = TextButton(1, Vec2(10 * 16, 0), "About", about, 0);

    fileMenu[0] = TextButton(2, Vec2(0, 1 * 16), "New File    ", newFile, 0);
    fileMenu[1] = TextButton(2, Vec2(0, 2 * 16), "Open File   ", openFile, 0);
    fileMenu[2] = TextButton(2, Vec2(0, 3 * 16), "Save File   ", saveFile, 0);
    fileMenu[3] = TextButton(2, Vec2(0, 4 * 16), "Save File As", saveFileAs, 0);

    editMenu[0] = TextButton(2, Vec2(5 * 16, 1 * 16), "New Palette   ", newPalette, 0);
    editMenu[1] = TextButton(2, Vec2(5 * 16, 2 * 16), "Remove Palette", removePalette, 0);
    editMenu[2] = TextButton(2, Vec2(5 * 16, 3 * 16), "Clear Palette", clearPalette, 0);

    hideFileMenu();
    hideEditMenu();
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

void showFileMenu() { for(int i = 0; i < 4; i++) fileMenu[i].visible = true; }
void hideFileMenu() { for(int i = 0; i < 4; i++) fileMenu[i].visible = false; }
void showEditMenu() { for(int i = 0; i < 3; i++) editMenu[i].visible = true; }
void hideEditMenu() { for(int i = 0; i < 3; i++) editMenu[i].visible = false; }

void newFile()
{
    cPalettelist.palettes.clear();
    for(int i = 0; i < 4; i++) colorVals[i].content = "000000FF";
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalettelist.palettes.push_back(cPalette);
    cPaletteID = 0;
}

void openFile()
{
    //Dialog with TextBox
}

void saveFile() { }
void saveFileAs() { }

void newPalette()
{
    if(cPalettelist.palettes.size() >= 70) return;
    for(int i = 0; i < 4; i++) colorVals[i].content = "000000FF";
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalettelist.palettes.push_back(cPalette);
    cPaletteID++;
}

void removePalette()
{
    if(cPalettelist.palettes.size() > 1)
    {
        cPaletteID -= 1;
        cPalettelist.palettes.pop_back();
        cPalette = cPalettelist.palettes[cPaletteID];
        for(int i = 0; i < 4; i++)
        {
            char tmpchr[9];

            sprintf(tmpchr, "%02x%02x%02x%02x", cPalette.col[i].r, cPalette.col[i].g, cPalette.col[i].b, cPalette.col[i].a);
            colorVals[i].content = tmpchr;
        }
    }
}

void clearPalette()
{
    for(int i = 0; i < 4; i++) colorVals[i].content = "000000FF";
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
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

    primary_visible = false;

    for(int i = 0; i < 4; i++) fileMenu[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 3; i++) editMenu[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 3; i++) menubar[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 4; i++) colorVals[i].inHandle(event, mousePressed, mousePos);

    for(int i = 0; i < 4; i++) std::replace(colorVals[i].content.begin(), colorVals[i].content.end(), '\0', '0');
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalettelist.palettes[cPaletteID] = cPalette;

    if(mousePressed && (mousePos.x < fileMenu[0].pos.x || mousePos.x >= fileMenu[0].pos.x + fileMenu[0].height * fileMenu[0].text.length() || mousePos.y < menubar[0].pos.y || mousePos.y >= fileMenu[3].pos.y + fileMenu[3].height)) hideFileMenu();
    if(mousePressed && (mousePos.x < editMenu[0].pos.x || mousePos.x >= editMenu[0].pos.x + editMenu[0].height * editMenu[0].text.length() || mousePos.y < menubar[1].pos.y || mousePos.y >= editMenu[2].pos.y + editMenu[2].height)) hideEditMenu();
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
    SDL_RenderClear(renderer);

    //Draw GUI
    for(int i = 0; i < 4; i++) colorValLabels[i].draw(renderer, guiSprites, guiPalettes);
    for(int i = 0; i < 4; i++) colorVals[i].draw(renderer, guiSprites, guiPalettes);

    //Draw Menubar
    menubarBG.draw(renderer, guiPalettes);
    for(int i = 0; i < 3; i++) menubar[i].draw(renderer, guiSprites, guiPalettes);
    for(int i = 0; i < 4; i++) fileMenu[i].draw(renderer, guiSprites, guiPalettes);
    for(int i = 0; i < 3; i++) editMenu[i].draw(renderer, guiSprites, guiPalettes);

    //Draw sample colors
    SDL_Rect tmp_rect;

    for(int i = 0; i < 4; i++)
    {
        iSDL_SetRenderDrawColor(renderer, cPalette.col[i]);
        tmp_rect = iSDL_Rect(2 * 16 + i * 9 * 16, 8 * 16, 8 * 16, 8 * 16);
        SDL_RenderFillRect(renderer, &tmp_rect);
    }

    //Draw Palletelist
    for(int n = 0; n < cPalettelist.palettes.size(); n++)
    {
        for(int i = 0; i < 4; i++)
        {
            iSDL_SetRenderDrawColor(renderer, cPalettelist.palettes[n].col[i]);
            tmp_rect = iSDL_Rect(2 * 16 + (n - (n >= 35 ? 35 : 0)) * 16, n >= 35 ? 25 * 16 + i * 16 : 20 * 16 + i * 16, 16, 16);
            SDL_RenderFillRect(renderer, &tmp_rect);
        }
    }

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