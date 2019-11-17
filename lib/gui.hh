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
    int longestEntryLength = 0;
    int cEntry = -1;
    int menuPosX = 0;
    int heightPerEntry;
    bool visible = false;

    MenuList() {  }
    MenuList(std::vector<std::string> iEntries, int iHeightPerEntry, int iMenuPosX)
    {
        entries = iEntries;
        heightPerEntry = iHeightPerEntry;
        menuPosX = iMenuPosX;

        for(int i = 0; i < entries.size(); i++)
        {
            if(entries[i].length() > longestEntryLength) longestEntryLength = entries[i].length();
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

        if(clicked)
        {
            
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
    MenuBar(std::vector<std::vector<std::string>> iEntries, int iHeight)
    {
        entries = iEntries[0];
        height = iHeight;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == 0) entryXPos.push_back(0);
            else entryXPos.push_back(entryXPos[i - 1] + (entries[i - 1].length() * height + height));

            MenuList tmp_menulist(iEntries[1 + i], iHeight, entryXPos[i]);
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

#endif // GUI_H