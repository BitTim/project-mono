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

	void flipColors(int pID, int c1, int c2)
	{
		SDL_Color tmp_col = palettes[pID].col[c1];
    palettes[pID].col[c1] = palettes[pID].col[c2];
  	palettes[pID].col[c2] = tmp_col;
	}

  int loadFile(const char* path)
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

		palette_file.close();
		return 0;
	}

	int saveFile(const char* path)
	{
		char tmp_word[2];
		char tmp_col[4];

		std::ofstream palette_file(path, std::ofstream::binary);
		if(!palette_file) return -1;

		tmp_word[0] = 0x54;
		tmp_word[1] = 0x50;
		palette_file.write(tmp_word, 2);

		nPalettes = palettes.size();
		tmp_word[0] = nPalettes & 0xFF00;
		tmp_word[1] = nPalettes & 0x00FF;
		palette_file.write(tmp_word, 2);

		for(int n = 0; n < nPalettes; n++)
		{
			for(int i = 0; i < 4; i++)
			{
				tmp_col[0] = palettes[n].col[i].r;
				tmp_col[1] = palettes[n].col[i].g;
				tmp_col[2] = palettes[n].col[i].b;
				tmp_col[3] = palettes[n].col[i].a;

				palette_file.write(tmp_col, 4);
			}
		}

		palette_file.close();
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