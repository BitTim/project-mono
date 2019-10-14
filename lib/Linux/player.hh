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

	void update(GameMap gamemap, Tilelist tilelist)
	{
		//Move X
		pos.x += dir.x * speed;

		//Collision X
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y        )][floor(pos.x        )]].collision) pos.x -= dir.x * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y        )][floor(pos.x + 0.99f)]].collision) pos.x -= dir.x * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y + 0.99f)][floor(pos.x        )]].collision) pos.x -= dir.x * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y + 0.99f)][floor(pos.x + 0.99f)]].collision) pos.x -= dir.x * speed;
		if(pos.x < 0 || pos.x >= gamemap.size[0] - 3) pos.x -= dir.x * speed;

		//Move Y
		pos.y += dir.y * speed;

		//Collision Y
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y        )][floor(pos.x        )]].collision) pos.y -= dir.y * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y        )][floor(pos.x + 0.99f)]].collision) pos.y -= dir.y * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y + 0.99f)][floor(pos.x        )]].collision) pos.y -= dir.y * speed;
		if(tilelist.tiles[gamemap.tile_data[floor(pos.y + 0.99f)][floor(pos.x + 0.99f)]].collision) pos.y -= dir.y * speed;
		if(pos.y < 0 || pos.y >= gamemap.size[1] - 3) pos.y -= dir.y * speed;
	}

	void draw_player(SDL_Renderer* renderer, Vec2f playerDrawOffset, Spritesheet pSprites, mono_palette palette)
	{
		pSprites.draw_sprite(renderer, 0, gtop(Vec2f(float((_TILESPERSCREENWIDTH) / 2) - playerDrawOffset.x, float((_TILESPERSCREENHEIGHT) / 2) - playerDrawOffset.y)), palette);
	}
};

#endif //PLAER_H
