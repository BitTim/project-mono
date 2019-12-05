#ifndef TCM_H
#define TCM_H

#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>

#include "tcp_file.hh"
#include "tcs_file.hh"
#include "tct_file.hh"

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
		if(header != 0x544D) return -1;

		//Get map size
		map_file.read(tmp_word, 2);
		size[0] = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(size[0] < _TILESPERSCREENWIDTH + 2) return -1;

		map_file.read(tmp_word, 2);
		size[1] = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(size[1] < _TILESPERSCREENHEIGHT + 2) return -1;

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
		map_file.close();
		return 0;
	}

	Vec2f draw_map(SDL_Renderer* renderer, Vec2f playerPosRaster, Tilelist tilelist, Spritesheet tile_sprites, Palettelist pal, word cPalette)
	{
		Vec2f playerOff;

		//Calculate the most upper left and most lower right tile positions
		Vec2f firstTile(playerPosRaster.x - float((_TILESPERSCREENWIDTH) / 2), playerPosRaster.y - float((_TILESPERSCREENHEIGHT) / 2));
		Vec2f lastTile(firstTile.x + _TILESPERSCREENWIDTH, firstTile.y + _TILESPERSCREENHEIGHT);

		//Calculate offsets
		if(firstTile.x < 0) playerOff.x = -firstTile.x;
		if(firstTile.y < 0) playerOff.y = -firstTile.y;

		if(lastTile.x > size[0] - 2) playerOff.x = -(lastTile.x - size[0] + 2);
		if(lastTile.y > size[1] - 2 + _LASTYTILEVISIBILITY) playerOff.y = -(lastTile.y - size[1] + 2 - _LASTYTILEVISIBILITY);

		Vec2f drawOff(playerPosRaster.x - floor(playerPosRaster.x), playerPosRaster.y - floor(playerPosRaster.y));

		//Clamp draw offset
		if(firstTile.x < 0 || lastTile.x >= size[0] - 2) drawOff.x = 0;
		if(firstTile.y < 0 || lastTile.y >= size[1] - 2 + _LASTYTILEVISIBILITY) drawOff.y = 0;

		//Clamp Camera to the map boundaries
		if(firstTile.x < 0) firstTile.x = 0;
		if(firstTile.y < 0) firstTile.y = 0;

		if(lastTile.x >= size[0] - 2) firstTile.x = size[0] - 2 - _TILESPERSCREENWIDTH;
		if(lastTile.y >= size[1] - 2 + _LASTYTILEVISIBILITY)
		{
			firstTile.y = size[1] - 2 - _TILESPERSCREENHEIGHT + _LASTYTILEVISIBILITY;
			drawOff.y = _LASTYTILEVISIBILITY;
		}

		//Draw tiles
		for(int y = 0; y < _TILESPERSCREENHEIGHT + 1; y++)
		{
			for(int x = 0; x < _TILESPERSCREENWIDTH + 1; x++)
			{
				tile_sprites.drawSprite(renderer, tilelist.tiles[tile_data[floor(firstTile.y) + y][floor(firstTile.x) + x]].spriteID, gtop(Vec2f(x - drawOff.x, y - drawOff.y)), pal, cPalette);
			}
		}

		if(drawOff.y > 0 && lastTile.y < size[1] - 1)
		{
			int y = _TILESPERSCREENHEIGHT + 1;
			for(int x = 0; x < _TILESPERSCREENWIDTH + 1; x++)
			{
				tile_sprites.drawSprite(renderer, tilelist.tiles[tile_data[floor(firstTile.y) + y][floor(firstTile.x) + x]].spriteID, gtop(Vec2f(x - drawOff.x, y - drawOff.y)), pal, cPalette);
			}
		}

		return playerOff;
	}
};

#endif /* end of include guard: TCM_H */

/*
NOTE: Map must be intendet size x + 2 and intended size y + 2 big, for prevention of segmentation faults

File Structure:
0x54 0x4D																																				Header (TM)
0x00 0x04																																				Map width (H Byte, L byte)
0x00 0x04																																				Map height (H Byte, L byte)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 0 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 1 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 2 (2 Bytes = 1 Tile)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00																					Data  Row 3 (2 Bytes = 1 Tile)
*/
