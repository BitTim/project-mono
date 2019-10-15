#ifndef TCT_H
#define TCT_H

#include <string>
#include <vector>
#include <fstream>
#include "datatypes.hh"

class Tile
{
public:
	word spriteID;
	word interactionID;
	bool collision;
	bool interactable;
	bool destroyable;
};

class Tilelist
{
public:
	word header;
	word nTiles;
	std::vector<Tile> tiles;

	int load_file(const char* path)
	{
		char tmp_word[2];
		Tile tmp_tile;
		char flags[1];

		std::ifstream tile_file(path, std::ifstream::binary);

		//Check if file is valid
		tile_file.read(tmp_word, 2);
		header = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(header != 0x5454) return -1;

		//Get number of tiles
		tile_file.read(tmp_word, 2);
		nTiles = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(nTiles == 0) return -1;

		//Populate the data
		for(int i = 0; i < nTiles; i++)
		{
			tile_file.read(tmp_word, 2);
			tmp_tile.spriteID = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);

			tile_file.read(tmp_word, 2);
			tmp_tile.interactionID = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);

			tile_file.read(flags, 1);
			tmp_tile.collision = (flags[0] >> 2) & 0x01 == 1 ? true : false;
			tmp_tile.interactable = (flags[0] >> 1) & 0x01 == 1 ? true : false;
			tmp_tile.destroyable = flags[0] & 0x01 == 1 ? true : false;

			tmp_tile.collision = false;

			tiles.push_back(tmp_tile);
		}
		return 0;
	}
};

#endif //TCT_H

/*

File Structure:
0x54 0x54																																				Header (TT)
0x00 0x01																																				Num Tiles (H Byte, L Byte)

0x00 0x01																																				Data 0, SpriteID
0x00 0x00																																				Data 0, InteractionID
0x05																																						Data 0, Flags (00000101)
																																								00000cid -> collision, interactable, destroyable
*/
