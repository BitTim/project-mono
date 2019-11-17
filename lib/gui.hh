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

class MenuBar
{
public:
    std::vector<std::string> entries;
    std::vector<int> entryXPos;
    int cEntry = -1;
    int height;
    bool visible = true;

    MenuBar() {  }
    MenuBar(std::vector<std::string> iEntires, int iHeight)
    {
        entries = iEntires;
        height = iHeight;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == 0) entryXPos.push_back(0);
            else entryXPos.push_back(entryXPos[i - 1] + (entries[i - 1].length() * height + height));
        }
    }

    void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
    {
        iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[2]);
        SDL_Rect bar = iSDL_Rect(0, 0, _SCREENRES[0], height);
        SDL_RenderFillRect(renderer, &bar);

        int posX = 0;
        SDL_Color tmp_col;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == cEntry || (i == cEntry + 1 && cEntry != -1))
            {
                tmp_col = pal.palettes[1].col[0];
                pal.palettes[1].col[0] = pal.palettes[1].col[3];
                pal.palettes[1].col[3] = tmp_col;
            }

            for(int j = 0; j < strlen(entries[i].c_str()); j++)
            {
                guiSprites.draw_sprite(renderer, char2sid(entries[i].c_str()[j]), Vec2(posX, 0), pal, 1, height / 16);
                posX += 16;
            }

            guiSprites.draw_sprite(renderer, char2sid(' '), Vec2(posX, 0), pal, 1, height / 16);
            posX += 16;
        }
    }

    void inHandle(Vec2 mousePos, bool clicked)
    {
        cEntry = -1;

        if(!visible) return;
        if(mousePos.y > height || height < 0) return;

        for(int i = 0; i < entries.size(); i++)
        {
            if(mousePos.x >= entryXPos[i] && mousePos.x < entryXPos[i] + entries[i].length() * height + height) cEntry = i;
        }
    }
};

#endif // GUI_H