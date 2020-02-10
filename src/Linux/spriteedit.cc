#include <iostream>
#include <string>
#include <vector>

#include "var.hh"
#include "gui.hh"
#include "datatypes.hh"

SDL_Window* window;
SDL_Rendrer* renderer;
SDL_Event event;
bool quit = false;

Spritesheet guiSprites;
Palettelist guiPalettes;
Palettelist spritePalettes;

Panel menubarBG;
TextButton menubar[2];
TextButton fileMenu[4];
TextButton editMenu[3];

TextButton prevPageButton;
TextButton nextPageButton;
Label pageLabel;

SpriteButton spritesheetPage[20];

Spritesheet cSpritesheet;

//================================
// onClick function templates
//================================

void showFileMenu();
void hideFileMenu();

void showEditMenu();
void hideEditMenu();

void newFile();
void openFileDialog();
void saveFileDialog();

void newSprite();
void removeSprite();
void clearSprite();

void changeCSprite();

void openFile();
void saveFile();
void saveFileAsDialog();
void cancelDialog();

//================================
// Utility functions
//================================

void init()
{
  _SCREENRES = Vec2(640, 496);

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

  //Clear the Window
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[0].col[0]);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  //Create GUI
  menubarBG = Panel(Vec2(0, 0), Vec2(_SCREENRES.x, 16), 3, 0);
  menubar[0] = TextButton(1, Vec2(0, 0), "File ", showFileMenu, 0);
  menubar[1] = TextButton(1, Vec2(5 * 16, 0), "Edit", showEditMenu, 0);
 
  fileMenu[0] = TextButton(2, Vec2(0, 1 * 16), "New File    ", newFile, 0);
  fileMenu[1] = TextButton(2, Vec2(0, 2 * 16), "Open File   ", openFileDialog, 0);
  fileMenu[2] = TextButton(2, Vec2(0, 3 * 16), "Save File   ", saveFileDialog, 0);
  fileMenu[3] = TextButton(2, Vec2(0, 4 * 16), "Save File As", saveFileAsDialog, 0);

  editMenu[0] = TextButton(2, Vec2(5 * 16, 1 * 16), "New Sprite   ", newSprite, 0);
  editMenu[1] = TextButton(2, Vec2(5 * 16, 2 * 16), "Remove Sprite", removeSprite, 0);
  editMenu[2] = TextButton(2, Vec2(5 * 16, 3 * 16), "Clear Sprite ", clearSprite, 0);

  prevPageButton = TextButton(1, Vec2(480, 370), "<", prevSpritesheetPage(), 0);
  nextPageButton = TextButton(1, Vec2(585, 370), ">", nextSpritesheetPage(), 0);

  dialogBG = Panel(Vec2(_SCREENRES.x / 2 - 32 * 16 / 2, _SCREENRES.y / 2 - 7 * 16 / 2), Vec2(32 * 16, 7 * 16), 3, 0);
  openDialogText = Label("Name of the file to open", Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 16), 0);
  saveDialogText = Label("Name of the file to save", Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 16), 0);
  dialogPath = TextBox(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 3 * 16), 0, 0, 30, 16);

  cancelButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Cancel", cancelDialog, 0);
  openButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 27 * 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Open", openFile, 0);
  saveButton = TextButton(2, Vec2(_SCREENRES.x / 2 - 32 * 16 / 2 + 27 * 16, _SCREENRES.y / 2 - 7 * 16 / 2 + 5 * 16), "Save", saveFile, 0);

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
// onClick functions
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
// Repeated functions
//================================

void update()
{

}

void draw()
{

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