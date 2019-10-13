#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "datatypes.hh"
#include "var.hh"

class Player
{
public:
	Vec2f pos;
	Vec2f dir;
	float speed;

	Player(Vec2f spawn = Vec2f(0.0f, 0.0f), float speed = 0.0f)
	{
		this->pos = spawn;
		this->speed = speed;
	}

	void update()
	{
		pos.x += dir.x * speed;
		pos.y += dir.y * speed;
	}

	void draw_player(SDL_Renderer* renderer, Vec2f playerDrawOffset, Spritesheet pSprites, mono_palette palette)
	{
		pSprites.draw_sprite(renderer, 0, gtop(Vec2f(pos.x - playerDrawOffset.x, pos.y - playerDrawOffset.y)), palette);
	}
};

#endif //PLAER_H
