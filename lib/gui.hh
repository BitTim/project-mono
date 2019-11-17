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
    int cEntry = -1;
    int height;
    bool visible = true;

    MenuBar() {  }
    MenuBar(std::vector<std::string> iEntires, int iHeight)
    {
        entries = iEntires;
        height = iHeight;
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

            posX += 16;
        }
    }

    void inHandle(Vec2 mousePos, bool clicked)
    {
        if(!visible) return;
        if(mousePos.y > height || height < 0) return;
        
        cEntry = -1;

        for(int i = 0; i < entries.size(); i++)
        {
            if(i == 0) if(mousePos.x >= 0 && mousePos.x < entries[i].length() * height) cEntry = i;
            if(mousePos.x >= entries[i - 1].length() * height && mousePos.x < entries[i].length() * height) cEntry = i;
        }

        printf("MousePos: %d %d   Active entry: %d\n", mousePos.x, mousePos.y, cEntry);
    }
};

#endif // GUI_H