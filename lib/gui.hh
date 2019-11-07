#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>

#include "tcs_file.hh"
#include "datatypes.hh"
#include "var.hh"

void inHandle()
{
    
}

void drawMenuBar(SDL_Renderer* renderer, int nEntries, char* entries[], int cEntry, int height, Spritesheet guiSprites, Palettelist pal)
{
    iSDL_SetRenderDrawColor(renderer, pal.palettes[1].col[2]);
    SDL_Rect bar = iSDL_Rect(0, 0, _SCREENRES[0], height);
    SDL_RenderFillRect(renderer, &bar);

    int posX = 0;
    SDL_Color tmp_col;

    for(int i = 0; i < nEntries; i++)
    {
        if(i == cEntry || (i == cEntry + 1 && cEntry != -1))
        {
            tmp_col = pal.palettes[1].col[0];
            pal.palettes[1].col[0] = pal.palettes[1].col[3];
            pal.palettes[1].col[3] = tmp_col;
        }

        for(int j = 0; j < strlen(entries[i]); j++)
        {
            guiSprites.draw_sprite(renderer, char2sid(entries[i][j]), Vec2(posX, 0), pal, 1, height / 16);
            posX += 16;
        }

        posX += 16;
    }
}

#endif // GUI_H