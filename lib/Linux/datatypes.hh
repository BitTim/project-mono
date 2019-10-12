#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "var.hh"

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

struct vec2
{
	int x;
	int y;

	vec2(int ix, int iy)
	{
		x = ix;
		y = iy;
	}
};

struct vec2f
{
	float x;
	float y;

	vec2f(float ix, float iy)
	{
		x = ix;
		y = iy;
	}
};

SDL_Color iSDL_Color(byte r, byte g, byte b, byte a)
{
	SDL_Color tmp;
	tmp.r = r;
	tmp.g = g;
	tmp.b = b;
	tmp.a = a;
	return tmp;
}

SDL_Rect iSDL_Rect(int x, int y, int w, int h)
{
	SDL_Rect tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.w = w;
	tmp.h = h;
	return tmp;
}

void iSDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color col)
{
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
}

struct mono_palette
{
	SDL_Color fg;
	SDL_Color bg;

	mono_palette(SDL_Color c1, SDL_Color c2)
	{
		fg = c1;
		bg = c2;
	}

	mono_palette(byte r1, byte g1, byte b1, byte a1, byte r2, byte g2, byte b2, byte a2)
	{
		fg = iSDL_Color(r1, g1, b1, a1);
		bg = iSDL_Color(r2, g2, b2, a2);
	}
};

vec2f ptog(vec2 pix) { return vec2f(float(pix.x / _SPRITESIZE), float(pix.y / _SPRITESIZE)); }
vec2 gtop(vec2f grid) { return vec2(int(grid.x * float(_SPRITESIZE)), int(grid.y * float(_SPRITESIZE))); }

#endif //DATATYPES_H
