#ifndef MOM_H
#define MOM_H

#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include "mos_file.hh"

class GameMap
{
public:
	word header;
	word size[2];
	std::vector<std::vector<word>> tile_data;

	int load_file(const char* path)
	{
		char tmp_word[2];
		std::vector<word> tmp_row;

		std::ifstream map_file(path, std::ifstream::binary);

		//Check if file is valid
		map_file.read(tmp_word, 2);
		header = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(header != 0x4D4D) return -1;

		//Get map size
		map_file.read(tmp_word, 2);
		size[0] = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(size[0] < _TILESPERSCREENWIDTH + 1) return -1;

		map_file.read(tmp_word, 2);
		size[1] = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(size[1] < _TILESPERSCREENHEIGHT + 1) return -1;

		//Populate the data
		for(int n = 0; n < size[1]; n++)
		{
			for(int i = 0; i < size[0]; i++)
			{
				map_file.read(tmp_word, 2);
				tmp_row.push_back(((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF));
			}
			tile_data.push_back(tmp_row);
			tmp_row.clear();
		}
		return 0;
	}

	vec2f draw_map(SDL_Renderer* renderer, vec2f playerPosRaster, Spritesheet tile_sprites, mono_palette palette)
	{
		//Calculate the most upper left tile position
		vec2f firstTile(playerPosRaster.x - float((_TILESPERSCREENWIDTH - 1) / 2), playerPosRaster.y - float((_TILESPERSCREENHEIGHT - 1) / 2));
		vec2f playerOff(firstTile.x < 0 ? firstTile.x : playerPosRaster.x - float((_TILESPERSCREENWIDTH - 1) / 2), firstTile.y < 0 ? firstTile.y : playerPosRaster.y - float((_TILESPERSCREENHEIGHT - 1) / 2));
		vec2 drawOff(firstTile.x - int(firstTile.x), firstTile.y - int(firstTile.y));

		//Clamp first tile to 0
		if(firstTile.x < 0) firstTile.x = 0;
		if(firstTile.y < 0) firstTile.y = 0;

		//Draw tiles
		for(int y = 0; y < _TILESPERSCREENHEIGHT + 1; y++)
		{
			for(int x = 0; x < _TILESPERSCREENWIDTH + 1; x++)
			{
				tile_sprites.draw_sprite(renderer, tile_data[floor(firstTile.y) + y][floor(firstTile.x) + x], gtop(vec2f(x - drawOff.x, y - drawOff.y)), palette);
			}
		}

		return playerOff;
	}
};

#endif /* end of include guard: MOM_H */

/*
File Structure:
0x4D 0x4D																																				Header (MM)
0x00 0x04																																				Map width (H Byte, L byte)
0x00 0x04																																				Map height (H Byte, L byte)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 0 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 1 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 2 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 3 (2 Bytes = 1 Tile)
*/
