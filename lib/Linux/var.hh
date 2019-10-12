#ifndef VAR_H
#define VAR_H

#include <math.h>

int _SCREENRES[2] = {640, 480};
bool _FULLSCREEN = false;
int _TILESPERSCREENWIDTH = 10;
int _PIXELSCALE = (_SCREENRES[0] / _TILESPERSCREENWIDTH) / 16;
int _TILESPERSCREENHEIGHT = ceil(_SCREENRES[1] / (_PIXELSCALE * 16));
int _SPRITESIZE = 16;

#endif //VAR_H
