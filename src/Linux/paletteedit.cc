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

ButtonLogic paletteSelect[70];
int cSelectorButton;

std::string openPath = "\0";
std::string savePath = "\0";

Panel dialogBG;
Label openDialogText;
Label saveDialogText;
TextBox dialogPath;

TextButton cancelButton;
TextButton openButton;
TextButton saveButton;

//================================
// onClick Function templates
//================================

void showFileMenu();
void hideFileMenu();

void showEditMenu();
void hideEditMenu();

void newFile();
void openFileDialog();
void saveFileDialog();

void newPalette();
void removePalette();
void clearPalette();

void about();

void changeCPalette();

void openFile();
void saveFile();
void saveFileAsDialog();
void cancelDialog();

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

    //TEMPORARY until new GUI Sprites: Swap colors 0 and 1 in palette 1 (Hover and Foreground color)
    guiPalettes.flipColors(1, 0, 1);

    //TEMPORARY until new GUI Sprites: Swap colors 0 from palette 0 and color 0 from palette 1 (Transparent BG and Hover)
    SDL_Color tmpcol;
    tmpcol = guiPalettes.palettes[0].col[0];
    guiPalettes.palettes[0].col[0] = guiPalettes.palettes[1].col[0];
    guiPalettes.palettes[1].col[0] = tmpcol;

    //Clear the Window
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[0].col[1]);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //Initialize GUI
    for(int i = 0; i < 4; i++)
    {
        colorValLabels[i] = Label("Color " + std::to_string(i), Vec2(2 * 16 + i * 9 * 16 + 16 / 2, 4 * 16), 0);
        colorVals[i] = TextBox(1, Vec2(2 * 16 + i * 9 * 16, 6 * 16), 2, 0, 8, 16, "000000FF");
    }

    cPalettelist.palettes.push_back(cPalette);

    menubarBG = Panel(Vec2(0, 0), Vec2(_SCREENRES.x, 16), 3, 0);
    menubar[0] = TextButton(1, Vec2(0, 0), "File", showFileMenu, 0);
    menubar[1] = TextButton(1, Vec2(5 * 16, 0), "Edit", showEditMenu, 0);
    menubar[2] = TextButton(1, Vec2(10 * 16, 0), "About", about, 0);

    fileMenu[0] = TextButton(2, Vec2(0, 1 * 16), "New File    ", newFile, 0);
    fileMenu[1] = TextButton(2, Vec2(0, 2 * 16), "Open File   ", openFileDialog, 0);
    fileMenu[2] = TextButton(2, Vec2(0, 3 * 16), "Save File   ", saveFileDialog, 0);
    fileMenu[3] = TextButton(2, Vec2(0, 4 * 16), "Save File As", saveFileAsDialog, 0);

    editMenu[0] = TextButton(2, Vec2(5 * 16, 1 * 16), "New Palette   ", newPalette, 0);
    editMenu[1] = TextButton(2, Vec2(5 * 16, 2 * 16), "Remove Palette", removePalette, 0);
    editMenu[2] = TextButton(2, Vec2(5 * 16, 3 * 16), "Clear Palette ", clearPalette, 0);

    dialogBG = Panel(Vec2(_SCREENRES.x / 2 - 32 * 16 / 2, _SCREENRES.y / 2 - 7 * 16 / 2), Vec2(32 * 16, 7 * 16), 3, 0);
    openDialogText = Label("Name of the file to open", Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 16), 0);
    saveDialogText = Label("Name of the file to save", Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 16), 0);
    dialogPath = TextBox(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 3 * 16), 0, 0, 30, 16);

    cancelButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Cancel", cancelDialog, 0);
    openButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 26 * 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Open", openFile, 0);
    saveButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 26 * 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Save", saveFile, 0);

    for(int i = 0; i < 70; i++)
    {
        int y;
        paletteSelect[i] = ButtonLogic(1, Vec2(2 * 16 + (i - (i >= 35 ? 35 : 0)) * 16, i >= 35 ? 25 * 16 : 20 * 16), Vec2(16, 16 * 4), 0, changeCPalette);        
        paletteSelect[i].visible = false;
    }

    paletteSelect[0].visible = true;

    cancelDialog();
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

void openFileDialog()
{
    hideFileMenu();

    dialogBG.visible = true;
    openDialogText.visible = true;
    dialogPath.visible = true;
    cancelButton.visible = true;
    openButton.visible = true;
}

void saveFileDialog()
{
    hideFileMenu();
    if(savePath != "\0")
    {
        saveFile();
        return;
    }

    dialogBG.visible = true;
    saveDialogText.visible = true;
    dialogPath.visible = true;
    cancelButton.visible = true;
    saveButton.visible = true;
}

void newPalette()
{
    if(cPalettelist.palettes.size() >= 70) return;
    for(int i = 0; i < 4; i++) colorVals[i].content = "000000FF";
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalettelist.palettes.push_back(cPalette);
    cPaletteID = cPalettelist.palettes.size() - 1;
    paletteSelect[cPaletteID].visible = true;
}

void removePalette()
{
    if(cPalettelist.palettes.size() > 1)
    {
        paletteSelect[cPalettelist.palettes.size() - 1].visible = false;
        cPalettelist.palettes.erase(cPalettelist.palettes.begin() + cPaletteID);

        if(cPaletteID < cPalettelist.palettes.size() - 1) cPaletteID += 1;
        else cPaletteID -= 1;

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

void changeCPalette() {
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalette = cPalettelist.palettes[cSelectorButton];
    cPaletteID = cSelectorButton;
    for(int i = 0; i < 4; i++)
    {
        char tmpchr[9];
        sprintf(tmpchr, "%02x%02x%02x%02x", cPalette.col[i].r, cPalette.col[i].g, cPalette.col[i].b, cPalette.col[i].a);
        colorVals[i].content = tmpchr;
    }
}

void openFile()
{
    newFile();
    openPath = dialogPath.content.c_str();
    savePath = openPath.c_str();

    char tmpchr[31];
    sprintf(tmpchr, "pedit_out/%s.tcp", openPath.c_str());

    if(cPalettelist.loadFile(tmpchr) == -1) printf("[F] Error 201: Failed to load palettelist \"%s\"\n", tmpchr);
    for(int i = 0; i < cPalettelist.palettes.size(); i++) paletteSelect[i].visible = true; 
    removePalette();

    cSelectorButton = 0;
    changeCPalette();
    cancelDialog();
}

void saveFile()
{
    if(savePath == "\0") savePath = dialogPath.content.c_str();
    char tmpchr[31];
    sprintf(tmpchr, "pedit_out/%s.tcp", savePath.c_str());
    cPalettelist.saveFile(tmpchr);
    cancelDialog();
}

void saveFileAsDialog()
{
    savePath = "\0";
    saveFileDialog();
}

void cancelDialog()
{
    dialogBG.visible = false;
    openDialogText.visible = false;
    saveDialogText.visible = false;
    dialogPath.visible = false;
    cancelButton.visible = false;
    openButton.visible = false;
    saveButton.visible = false;
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

    primary_visible = false;

    dialogPath.inHandle(event, mousePressed, mousePos);
    cancelButton.inHandle(event, mousePressed, mousePos);
    openButton.inHandle(event, mousePressed, mousePos);
    saveButton.inHandle(event, mousePressed, mousePos);

    for(int i = 0; i < 4; i++) fileMenu[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 3; i++) editMenu[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 3; i++) menubar[i].inHandle(event, mousePressed, mousePos);
    for(int i = 0; i < 4; i++) colorVals[i].inHandle(event, mousePressed, mousePos);

    for(int i = 0; i < 70; i++)
    {
        cSelectorButton = i;
        paletteSelect[i].inHandle(event, mousePressed, mousePos);
    }

    for(int i = 0; i < 4; i++) std::replace(colorVals[i].content.begin(), colorVals[i].content.end(), '\0', '0');
    for(int i = 0; i < 4; i++) cPalette.col[i] = iSDL_Color((std::stoul(colorVals[i].content, nullptr, 16) & 0xFF000000) >> 24, (std::stoul(colorVals[i].content, nullptr, 16) & 0x00FF0000) >> 16, (std::stoul(colorVals[i].content, nullptr, 16) & 0x0000FF00) >> 8, std::stoul(colorVals[i].content, nullptr, 16) & 0x000000FF);
    cPalettelist.palettes[cPaletteID] = cPalette;

    if(mousePressed && (mousePos.x < fileMenu[0].pos.x || mousePos.x >= fileMenu[0].pos.x + fileMenu[0].height * fileMenu[0].text.length() || mousePos.y < menubar[0].pos.y || mousePos.y >= fileMenu[3].pos.y + fileMenu[3].height)) hideFileMenu();
    if(mousePressed && (mousePos.x < editMenu[0].pos.x || mousePos.x >= editMenu[0].pos.x + editMenu[0].height * editMenu[0].text.length() || mousePos.y < menubar[1].pos.y || mousePos.y >= editMenu[2].pos.y + editMenu[2].height)) hideEditMenu();
}

void draw()
{
    iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[0].col[1]);
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
    paletteSelect[cPaletteID].draw(renderer, guiPalettes);

    //Draw dialog
    dialogBG.draw(renderer, guiPalettes);
    openDialogText.draw(renderer, guiSprites, guiPalettes);
    saveDialogText.draw(renderer, guiSprites, guiPalettes);
    dialogPath.draw(renderer, guiSprites, guiPalettes);
    cancelButton.draw(renderer, guiSprites, guiPalettes);
    openButton.draw(renderer, guiSprites, guiPalettes);
    saveButton.draw(renderer, guiSprites, guiPalettes);

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