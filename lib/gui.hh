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
#include "keyin.hh"

// +--------------------------------+
// | Current GUI classes:           |
// | - Plane                        |
// | - TextBox                      |
// | - Label                        |
// +--------------------------------+

// +--------------------------------+
// | guiLayer is the layer on which |
// | the UI Element is placed.      |
// | guiLayer can be:               |
// | - 0 -> Static(Background)      |
// | - 1 -> Dynamic(Middle)         |
// | - 2 -> Priority(Foreground)    |
// |                                |
// | UI Elements excluded from      |
// | layer 0:                       |
// | - TextBox                      |
// +--------------------------------+

//================================
// Plane
//================================
// Default guiLayer: 0
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================
class Plane
{
public:
    Vec2 size;
    Vec2 pos;
    byte guiLayer;
    byte colorID;
    byte guiPaletteID;
    bool visible = true;

    Plane() { }
    Plane(Vec2 iSize, vec2 iPos, byte iColorID, byte iGuiPaletteID)
    {
        size = iSize;
        pos = iPos;
        colorID = iColorID;
        guiPaletteID = iGuiPaletteID;
    }

    void draw(SDL_Renderer* renderer, Palettelist guiPalettes)
    {
        iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[guiPaletteID].col[colorID]);
        SDL_Rect plane = iSDL_Rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
        SDL_RenderFillRect(renderer, &plane);
    }

}

//================================
// TextBox
//================================
// Default guiLayer: 1
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================

class TextBox
{
public:
	int height;
	int maxInLength;
	byte inMode;             //0 = Text, 1 = Num Dec, 2 = Num Hex
    byte guiLayer;
    byte guiPaletteID;
	Vec2 pos;
    int cursorPos = 0;
	std::string content;
	bool visible = true;
    bool focused = false;

	TextBox() { }
	TextBox(byte iGuiLayer, byte iInMode, Vec2 iPos, byte iGuiPaletteID, std::string iContent = "\0", int iMaxInLength = 10, int iHeight = 16)
	{
        guiLayer = iGuiLayer;
        if(guiLayer == 0) printf("[W] Warning: guiLayer set to 0 for \"TextBox\". This UI Element would never be focused\n");

        guiPaletteID = iGuiPaletteID;

		inMode = iInMode;
		pos = iPos;
		height = iHeight;
		maxInLength = iMaxInLength;

        content = iContent;
        content.resize(maxInLength, '\0');
	}

	void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist guiPalettes)
	{
		if(!visible) return;

        //Draw Background
        Panel background(pos, Vec2(maxInLength * heigth, height), 2, guiPaletteID);
        background.draw(renderer, guiPalettes);

        //Draw Content
        for(int i = 0; i < maxInLength; i++)
        {
            if(content[i] == '\0') break;
            guiSprites.drawSprite(renderer, char2sid(content[i]), Vec2(pos.x + i * height, pos.y), pal, 1, height / 16);
        }

        //Draw Border
        iSDL_SetRenderDrawColor(renderer, focused ? pal.palettes[1].col[3] : pal.palettes[1].col[2]);
		SDL_RenderDrawRect(renderer, &panel);
	}

    void inHandle(SDL_Event event, bool mousePressed, Vec2 mousePos)
    {
        if(!visible) return;
        if(guiLayer == 0) return;

        if(mousePressed)
        {
            if(mousePos.x >= pos.x && mousePos.x < pos.x + height * maxInLength && mousePos.y >= pos.y && mousePos.y < pos.y + height) focused = true;
            else focused = false;
        }

        if(!focused) return;

        if(event.type == SDL_KEYDOWN)
        {
            char tmpchr = event2char(event, inMode);
            if(cursorPos < maxInLength && tmpchr != '\0') content[cursorPos++] = tmpchr;

            if(event.key.keysym.sym == SDLK_BACKSPACE)
            {
                cursorPos -= 1;
                content[cursorPos] = '\0';
            }

            if(cursorPos > maxInLength) cursorPos = maxInLength;
            if(cursorPos < 0) cursorPos = 0;
        }
    }
};

//================================
// Label
//================================
// Default guiLayer: 0
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================

class Label
{
public:
	int height;
	Vec2 pos;
	std::string content;
	bool visible = true;

	Label() { }
	Label(std::string iContent, Vec2 iPos, int iHeight = 16)
	{
		content = iContent;
        pos = iPos;
        height = iHeight;
	}

	void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist pal)
	{
		if(!visible) return;
        
        //Draw Content
        for(int i = 0; i < content.length(); i++)
        {
            guiSprites.drawSprite(renderer, char2sid(content[i]), Vec2(pos.x + i * height, pos.y), pal, 1, height / 16);
        }
	}
};

#endif // GUI_H
