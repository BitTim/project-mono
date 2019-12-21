#ifndef VAR_H
#define VAR_H

#include <math.h>
#include "datatypes.hh"

Vec2 _SCREENRES = Vec2(640, 480);
bool _FULLSCREEN = false;
int _TILESPERSCREENWIDTH = 10;
int _PIXELSCALE = (_SCREENRES.x / _TILESPERSCREENWIDTH) / 16;
float _TILESPERSCREENHEIGHT = ceil(float(_SCREENRES.y) / (float(_PIXELSCALE) * 16.0f));
float _LASTYTILEVISIBILITY = _TILESPERSCREENHEIGHT - (float(_SCREENRES.y) / (float(_PIXELSCALE) * 16.0f));
int _SPRITESIZE = 16;

#endif //VAR_H
