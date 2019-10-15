#ifndef TCP_H
#define TCP_H

#include <vector>
#include <fstream>
#include "datatypes.hh"

class Palette
{
public:
	SDL_Color col[4];
};

class Palettelist
{
public:
  word header;
  word nPalettes;
  std::vector<Palette> palettes;

  int load_file(const char* path)
	{
		char tmp_word[2];
    char tmp_col[4];
		Palette tmp_palette;

		std::ifstream palette_file(path, std::ifstream::binary);

		palette_file.read(tmp_word, 2);
		header = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(header != 0x5450) return -1;

		palette_file.read(tmp_word, 2);
		nPalettes = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(nPalettes == 0) return -1;

		for(int n = 0; n < nPalettes; n++)
		{
			for(int i = 0; i < 4; i++)
			{
				palette_file.read(tmp_col, 4);
				tmp_palette.col[i] = iSDL_Color(tmp_col[0], tmp_col[1], tmp_col[2], tmp_col[3]);
			}
			palettes.push_back(tmp_palette);
		}
		return 0;
	}
};

#endif //TCP_H

/*
File Structure:
0x54 0x50                                                                               Header (TP)
0x00 0x04                                                                               Amount of palettes
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00         Data 0 (RGBA, RGBA, RGBA, RGBA) 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00         Data 1 (RGBA, RGBA, RGBA, RGBA) 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00         Data 2 (RGBA, RGBA, RGBA, RGBA) 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00         Data 3 (RGBA, RGBA, RGBA, RGBA) 
*/