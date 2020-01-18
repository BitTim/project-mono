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
// | - Panel                        |
// | - ButtonLogic                  |
// | - TextBox                      |
// | - TextButton                   |
// | - SpriteButton                 |
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

bool primary_visible = false;

//================================
// Panel
//================================
// Default guiLayer: 0
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================
class Panel
{
public:
    Vec2 size;
    Vec2 pos;
    byte colorID;
    byte guiPaletteID;
    bool visible = true;

    Panel() { }
    Panel(Vec2 iPos, Vec2 iSize, byte iColorID, byte iGuiPaletteID)
    {
        size = iSize;
        pos = iPos;
        colorID = iColorID;
        guiPaletteID = iGuiPaletteID;
    }

    void draw(SDL_Renderer* renderer, Palettelist guiPalettes)
    {
        if(!visible) return;
        
        iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[guiPaletteID].col[colorID]);
        SDL_Rect plane = iSDL_Rect(pos.x, pos.y, size.x, size.y);
        SDL_RenderFillRect(renderer, &plane);
    }
};

//================================
// ButtonLogic
//================================
// Default guiLayer: 1
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================
class ButtonLogic
{
public:
    Vec2 pos;
    Vec2 size;
    byte guiLayer;
    byte guiPaletteID;
    bool visible = true;
    bool pressed = false;
    bool hovered = false;
    void (*onClick)();

    ButtonLogic() { }
    ButtonLogic(byte iGuiLayer, Vec2 iPos, Vec2 iSize, byte iGuiPaletteID, void(*iOnClick)())
    {
        guiLayer = iGuiLayer;
        pos = iPos;
        size = iSize;
        onClick = iOnClick;
        guiPaletteID = iGuiPaletteID;
    }

    void draw(SDL_Renderer* renderer, Palettelist guiPalettes)
	{
		if(!visible) return;
    
        //Draw Border
        byte color;
        if(hovered) color = 6;
        else if(pressed) color = 7;
        else color = 4;

        iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[color > 3 ? guiPaletteID + 1 : guiPaletteID].col[color > 3 ? color - 4 : color]);
		SDL_Rect plane = iSDL_Rect(pos.x, pos.y, size.x, size.y);
        SDL_RenderDrawRect(renderer, &plane);
	}

    void inHandle(SDL_Event event, bool mousePressed, Vec2 mousePos)
    {
        if(visible && guiLayer == 2) primary_visible = true;

        if(!visible) return;
        if(guiLayer == 0) return;
        if(guiLayer == 1 && primary_visible) return;

        if(mousePos.x > pos.x && mousePos.x < pos.x + size.x && mousePos.y > pos.y && mousePos.y < pos.y + size.y) hovered = true;
        else hovered = false;

        if(mousePressed)
        {
            if(mousePos.x > pos.x && mousePos.x < pos.x + size.x && mousePos.y > pos.y && mousePos.y < pos.y + size.y)
            {
                pressed = true;
                (*onClick)();
            }
        } 

        if(event.type == SDL_MOUSEBUTTONUP) pressed = false;
    }
};

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
    std::string iContent;
	bool visible = true;
    bool focused = false;
    bool hovered = false;

	TextBox() { }
	TextBox(byte iGuiLayer, Vec2 iPos, byte iInMode, byte iGuiPaletteID, int iMaxInLength = 10, int iHeight = 16, std::string iIContent = "\0")
	{
        guiLayer = iGuiLayer;
        if(guiLayer == 0) printf("[W] Warning: guiLayer set to 0 for \"TextBox\". This UI Element would never be focused\n");

        guiPaletteID = iGuiPaletteID;

		inMode = iInMode;
		pos = iPos;
		height = iHeight;
		maxInLength = iMaxInLength;

        content = iIContent;
        iContent = iIContent;
        content.resize(maxInLength, '\0');
	}

	void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist guiPalettes)
	{
		if(!visible) return;

        //Draw Background
        Panel background(pos, Vec2(maxInLength * height, height), 2, guiPaletteID);
        background.draw(renderer, guiPalettes);

        //Draw Content
        for(int i = 0; i < maxInLength; i++)
        {
            if(content[i] == '\0') break;
            guiSprites.drawSprite(renderer, char2sid(content[i]), Vec2(pos.x + i * height, pos.y), guiPalettes, guiPaletteID + 1, height / 16);
        }

        //Draw Border
        byte color; 
        if(focused) color = 7;
        else if(hovered) color = 6;
        else color = 3;

        iSDL_SetRenderDrawColor(renderer, guiPalettes.palettes[color > 3 ? guiPaletteID + 1 : guiPaletteID].col[color > 3 ? color - 4 : color]);
		SDL_Rect plane = iSDL_Rect(pos.x, pos.y, maxInLength * height, height);
        SDL_RenderDrawRect(renderer, &plane);
	}

    void inHandle(SDL_Event event, bool mousePressed, Vec2 mousePos)
    {
        if(visible && guiLayer == 2) primary_visible = true;

        if(!visible)
        {
            content = iContent;
            return;
        }

        if(guiLayer == 0) return;
        if(primary_visible && guiLayer != 2)
        {
            focused = false;
            return;
        }

        if(mousePos.x >= pos.x && mousePos.x < pos.x + height * maxInLength && mousePos.y >= pos.y && mousePos.y < pos.y + height) hovered = true;
        else hovered = false;
        if(mousePressed) focused = hovered;

        if(!focused)
        {
            cursorPos = 0;
            return;
        }

        if(event.type == SDL_KEYDOWN)
        {
            char tmpchr = event2char(event, inMode);
            if(cursorPos < maxInLength && cursorPos >= 0 && tmpchr != '\0')
            {
                content[cursorPos] = tmpchr;
                cursorPos += 1;
            }

            if(event.key.keysym.sym == SDLK_BACKSPACE)
            {
                if(cursorPos >= 0) cursorPos -= 1;
                content[cursorPos] = '\0';
            }

            if(cursorPos > maxInLength) cursorPos = maxInLength;
            if(cursorPos < 0) cursorPos = 0;
        }
    }
};

//================================
// TextButton
//================================
// Default guiLayer: 1
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================
class TextButton
{
public:
    Vec2 pos;
    std::string text;
    int height;
    byte guiLayer;
    byte guiPaletteID;
    bool visible = true;
    bool pressed = false;
    bool hovered = false;
    void (*onClick)();

    TextButton() { }
    TextButton(byte iGuiLayer, Vec2 iPos, std::string iText, void (*iOnClick)(), byte iGuiPaletteID, int iHeight = 16)
    {
        guiLayer = iGuiLayer;
        pos = iPos;
        text = iText;
        guiPaletteID = iGuiPaletteID;

        height = iHeight;
        onClick = iOnClick;
    }

    void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist guiPalettes)
    {
        if(!visible) return;

        //Draw Background
        byte color; 
        if(pressed) color = 7;
        else if(hovered) color = 6;
        else color = 3;

        Panel background(pos, Vec2(text.length() * height, height), color > 3 ? color - 4 : color, color > 3 ? guiPaletteID + 1 : guiPaletteID);
        background.draw(renderer, guiPalettes);

        //Draw Content
        for(int i = 0; i < text.length(); i++)
        {
            guiSprites.drawSprite(renderer, char2sid(text[i]), Vec2(pos.x + i * height, pos.y), guiPalettes, guiPaletteID + 1, height / 16);
        }
    }

    void inHandle(SDL_Event event, bool mousePressed, Vec2 mousePos)
    {
        if(visible && guiLayer == 2) primary_visible = true;

        if(!visible) return;
        if(guiLayer == 0) return;
        if(guiLayer == 1 && primary_visible) return;

        if(mousePos.x > pos.x && mousePos.x < pos.x + text.length() * height && mousePos.y > pos.y && mousePos.y < pos.y + height) hovered = true;
        else hovered = false;

        if(mousePressed)
        {
            if(mousePos.x > pos.x && mousePos.x < pos.x + text.length() * height && mousePos.y > pos.y && mousePos.y < pos.y + height)
            {
                pressed = true;
                (*onClick)();
            }
        } 

        if(event.type == SDL_MOUSEBUTTONUP) pressed = false;
    }
};

//================================
// SpriteButton
//================================
// Default guiLayer: 1
// Can be overriden if utilised
// in a UI Element on a higher
// Layer
//================================
class SpriteButton
{
public:
    Vec2 pos;
    int height;
    byte guiLayer;
    Spritesheet sprites;
    int normalSprite;
    int hoveredSprite;
    int pressedSprite;
    int normalPaletteID;
    int hoveredPaletteID;
    int pressedPaletteID;
    bool visible = true;
    bool pressed = false;
    bool hovered = false;
    void (*onClick)();

    SpriteButton() { }
    SpriteButton(byte iGuiLayer, Vec2 iPos, Spritesheet iSprites, int iNormalSprite, int iHoveredSprite, int iPressedSprite, int iNormalPaletteID, int iHoveredPaletteID, int iPressedPaletteID, void (*iOnClick)(), int iHeight = 16)
    {
        guiLayer = iGuiLayer;
        pos = iPos;
        sprites = iSprites;

        normalSprite = iNormalSprite;
        hoveredSprite = iHoveredSprite;
        pressedSprite = iPressedSprite;

        normalPaletteID = iNormalPaletteID;
        hoveredPaletteID = iHoveredPaletteID;
        pressedPaletteID = iPressedPaletteID;

        height = iHeight;
        onClick = iOnClick;
    }

    void draw(SDL_Renderer* renderer, Palettelist pal)
    {
        if(!visible) return;

        int sprite;
        int paletteID;

        if(pressed)
        {
            sprite = pressedSprite;
            paletteID = pressedPaletteID;
        }
        else if(hovered)
        {
            sprite = hoveredSprite;
            paletteID = hoveredPaletteID;
        }
        else
        {
            sprite = normalSprite;
            paletteID = normalPaletteID;
        }

        sprites.drawSprite(renderer, sprite, Vec2(pos.x / (height / 16), pos.y / (height / 16)), pal, paletteID, height / 16);
    }

    void inHandle(SDL_Event event, bool mousePressed, Vec2 mousePos)
    {
        if(visible && guiLayer == 2) primary_visible = true;

        if(!visible) return;
        if(guiLayer == 0) return;
        if(guiLayer == 1 && primary_visible) return;

        if(mousePos.x > pos.x && mousePos.x < pos.x + height && mousePos.y > pos.y && mousePos.y < pos.y + height) hovered = true;
        else hovered = false;

        if(mousePressed)
        {
            if(mousePos.x > pos.x && mousePos.x < pos.x + height && mousePos.y > pos.y && mousePos.y < pos.y + height)
            {
                pressed = true;
                (*onClick)();
            }
        } 

        if(event.type == SDL_MOUSEBUTTONUP) pressed = false;
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
    byte guiPaletteID;
	std::string content;
	bool visible = true;

	Label() { }
	Label(std::string iContent, Vec2 iPos, byte iGuiPaletteID, int iHeight = 16)
	{
		content = iContent;
        pos = iPos;
        height = iHeight;

        guiPaletteID = iGuiPaletteID;
	}

	void draw(SDL_Renderer* renderer, Spritesheet guiSprites, Palettelist guiPalettes)
	{
		if(!visible) return;
        
        //Draw Content
        for(int i = 0; i < content.length(); i++)
        {
            if(content[i] == '\0') break;
            guiSprites.drawSprite(renderer, char2sid(content[i]), Vec2(pos.x + i * height, pos.y), guiPalettes, guiPaletteID + 1, height / 16);
        }
	}
};

#endif // GUI_H
