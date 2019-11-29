#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "tcs_file.hh"
#include "datatypes.hh"
#include "var.hh"

//================================
// Menu List
//================================

class MenuList
{
public:
    std::vector<std::string> entries;
    std::vector<onClickFunc> functions;
    int longestEntryLength = 0;
    int cEntry = -1;
    int menuPosX = 0;
    int heightPerEntry;
    bool visible = false;

    MenuList() {  }
    MenuList(std::vector<std::string> iEntries, std::vector<onClickFunc> iFunctions, int iHeightPerEntry, int iMenuPosX)
    {
        entries = iEntries;
        heightPerEntry = iHeightPerEntry;
        menuPosX = iMenuPosX;

        for(int i = 0; i < entries.size(); i++)
        {
            if(entries[i].length() > longestEntryLength) longestEntryLength = entries[i].length();
            functions.push_back(iFunctions[i]);
        }
    }

    void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
    {
        if(!visible) return;

        pal.flipColors(1, 0, 2);

        for(int i = 0; i < entries.size(); i++)
        {
            int posX = 0;
            entries[i].resize(longestEntryLength, ' ');

            if(i == cEntry) pal.flipColors(1, 0, 3);

            for(int j = 0; j < longestEntryLength; j++)
            {
                guiSprites.draw_sprite(renderer, char2sid(entries[i].c_str()[j]), Vec2(menuPosX + posX, heightPerEntry + i * heightPerEntry), pal, 1, heightPerEntry / 16);
                posX += 16;
            }

            if(i == cEntry) pal.flipColors(1, 0, 3);
        }

        pal.flipColors(1, 0, 2);
    }

    void inHandle(Vec2 mousePos, bool clicked)
    {
        cEntry = -1;

        if(!visible) return;
        if(mousePos.x > menuPosX + longestEntryLength * heightPerEntry || mousePos.x < menuPosX) return;
        if(mousePos.y > heightPerEntry + entries.size() * heightPerEntry || mousePos.y < heightPerEntry) return;

        for(int i = 0; i < entries.size(); i++)
        {
            if(mousePos.y >= heightPerEntry + (i * heightPerEntry) && mousePos.y < heightPerEntry + ((i + 1) * heightPerEntry)) cEntry = i;
        }

        if(clicked && cEntry != 1)
        {
            functions[cEntry]();
        }
    }
};

//================================
// Menu Bar
//================================

class MenuBar
{
public:
    std::vector<std::string> entries;
    std::vector<MenuList> menus;
    std::vector<int> entryXPos;
    int cEntry = -1;
    int height;
    bool visible = true;

    MenuBar() {  }
    MenuBar(std::vector<std::vector<std::string>> iEntries, std::vector<std::vector<onClickFunc>> iFunctions, int iHeight)
    {
        entries = iEntries[0];
        height = iHeight;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == 0) entryXPos.push_back(0);
            else entryXPos.push_back(entryXPos[i - 1] + (entries[i - 1].length() * height + height));

            MenuList tmp_menulist(iEntries[1 + i], iFunctions[i], iHeight, entryXPos[i]);
            menus.push_back(tmp_menulist);
        }
    }

    void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
    {
        iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[2]);
        SDL_Rect bar = iSDL_Rect(0, 0, _SCREENRES[0], height);
        SDL_RenderFillRect(renderer, &bar);

        int posX = 0;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == cEntry) pal.flipColors(1, 0, 3);

            for(int j = 0; j < strlen(entries[i].c_str()); j++)
            {
                guiSprites.draw_sprite(renderer, char2sid(entries[i].c_str()[j]), Vec2(posX, 0), pal, 1, height / 16);
                posX += 16;
            }

            guiSprites.draw_sprite(renderer, char2sid(' '), Vec2(posX, 0), pal, 1, height / 16);
            posX += 16;

            if(i == cEntry) pal.flipColors(1, 0, 3);

            menus[i].draw(renderer, guiSprites, pal);
        }
    }

    void inHandle(Vec2 mousePos, bool clicked)
    {
        if(!visible) return;
        cEntry = -1;

        for(int i = 0; i < entries.size(); i++)
        {
            if(mousePos.y >= 0 && mousePos.y < height) if(mousePos.x >= entryXPos[i] && mousePos.x < entryXPos[i] + entries[i].length() * height + height) cEntry = i;
            
            menus[i].inHandle(mousePos, clicked);
            if(clicked) if(menus[i].visible && i != cEntry) menus[i].visible = false; 
        }

        if(clicked)
        {
            if(cEntry != -1) menus[cEntry].visible = menus[cEntry].visible ? false : true;
        }
    }
};

//================================
// Simple Dialog Box
//================================

class SimpleDialogBox
{
public:
    std::string title;
    std::string content;
    int fontHeight;
    Vec2 gridSize;
    Vec2 pos;
    bool visible;

    SimpleDialogBox() { };
    SimpleDialogBox(std::string iTitle, std::string iContent, int iFontHeight, Vec2 iGridSize = Vec2(20, 5), Vec2 iPos = Vec2(-1, -1))
    {
        title = iTitle;
        content = iContent;
        fontHeight = iFontHeight;
        gridSize = iGridSize;

        if(iPos.x == -1 || iPos.y == -1) pos = Vec2(_SCREENRES[0] / 2 - (gridSize.x * fontHeight) / 2, _SCREENRES[1] - gridSize.y * fontHeight);
        else pos = iPos;
    }

    void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
    {
        if(!visible) return;

        iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[2]);
        SDL_Rect panel = iSDL_Rect(pos.x, pos.y, gridSize.x * fontHeight, gridSize.y * fontHeight);
        SDL_RenderFillRect(renderer, &panel);

        int contentIterator = 0;
        pal.flipColors(1, 0, 2);

        for(int i = 0; i < title.length(); i++) guiSprites.draw_sprite(renderer, char2sid(title[i]), Vec2(pos.x + (i * fontHeight), pos.y), pal, 1, fontHeight / 16);

        for(int y = 2; y < gridSize.y; y++)
        {
            for(int x = 0; x < gridSize.x; x++)
            {
                if(contentIterator < content.length())
                {
                    if(content[contentIterator] == '\n')
                    {
                        contentIterator++;
                        break;
                    }
                    
                    guiSprites.draw_sprite(renderer, char2sid(content[contentIterator]), Vec2(pos.x + (x * fontHeight), pos.y + (y * fontHeight)), pal, 1, fontHeight / 16);
                    contentIterator++;
                }
            }
        }
        pal.flipColors(1, 0, 2);
    }
};

//================================
// TextBox
//================================

class TextBox
{
public:
	int height;
	int maxInLength;
	int inMode;
	Vec2 pos;
	std::string content;
	bool visible = true;
    bool focused = false;

	TextBox() { }
	TextBox(int iInMode, Vec2 iPos, int iHeight = 16, int iMaxInLength = 10)
	{
		inMode = iInMode;
		pos = iPos;
		height = iHeight;
		maxInLength = iMaxInLength;
	}

	void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
	{
		if(!visible) return;

        //Draw Background
        iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[2]);
        SDL_Rect panel = iSDL_Rect(pos.x, pos.y, maxInLength * height, height);
        SDL_RenderFillRect(renderer, &panel);

        //Draw Content
        for(int i = 0; i < maxInLength; i++)
        {
            if(i >= content.length()) break;
            guiSprites.draw_sprite(renderer, char2sid(content[i], pos, pal, 1, height / 16));
        }

        //Draw Border
        iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[1]);
		SDL_RenderDrawRect(renderer, &panel);
	}

    void inHandle(SDL_Event event)
    {
        if(!visible) return;

        if(event.type == SDL_MOUSE)
    }
};

#endif // GUI_H
