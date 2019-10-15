#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "var.hh"

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

struct Vec2
{
	int x;
	int y;

	Vec2(int ix = 0, int iy = 0)
	{
		x = ix;
		y = iy;
	}
};

struct Vec2f
{
	float x;
	float y;

	Vec2f(float ix = 0.0f, float iy = 0.0f)
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

Vec2f ptog(Vec2 pix) { return Vec2f(float(pix.x / _SPRITESIZE), float(pix.y / _SPRITESIZE)); }
Vec2 gtop(Vec2f grid) { return Vec2(int(grid.x * float(_SPRITESIZE)), int(grid.y * float(_SPRITESIZE))); }

#endif //DATATYPES_H
