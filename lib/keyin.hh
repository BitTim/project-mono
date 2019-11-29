#ifndef KEYIN_H
#define KEYIN_H

#include <SDL2/SDL.h>

char event2char(SDL_Event event, int mode)
{
  char c;

  switch(event.key.keysym.sym)
  {
  case 'a':
      if(mode != 1) c = 'a';
      break;
  case 'b':
      if(mode != 1) c = 'b';
      break;
  case 'c':
      if(mode != 1) c = 'c';
      break;
  case 'd':
      if(mode != 1) c = 'd';
      break;
  case 'e':
      if(mode != 1) c = 'e';
      break;
  case 'f':
      if(mode != 1) c = 'f';
      break;
  case 'g':
      if(mode == 0) c = 'g';
      break;
  case 'h':
      if(mode == 0) c = 'h';
      break;
  case 'i':
      if(mode == 0) c = 'i';
      break;
  case 'j':
      if(mode == 0) c = 'j';
      break;
  case 'k':
      if(mode == 0) c = 'k';
      break;
  case 'l':
      if(mode == 0) c = 'l';
      break;
  case 'm':
      if(mode == 0) c = 'm';
      break;
  case 'n':
      if(mode == 0) c = 'n';
      break;
  case 'o':
      if(mode == 0) c = 'o';
      break;
  case 'p':
      if(mode == 0) c = 'p';
      break;
  case 'q':
      if(mode == 0) c = 'q';
      break;
  case 'r':
      if(mode == 0) c = 'r';
      break;
  case 's':
      if(mode == 0) c = 's';
      break;
  case 't':
      if(mode == 0) c = 't';
      break;
  case 'u':
      if(mode == 0) c = 'u';
      break;
  case 'v':
      if(mode == 0) c = 'v';
      break;
  case 'w':
      if(mode == 0) c = 'w';
      break;
  case 'x':
      if(mode == 0) c = 'x';
      break;
  case 'y':
      if(mode == 0) c = 'y';
      break;
  case 'z':
      if(mode == 0) c = 'z';
      break;
  case '0':
      if(mode != 0) c = '0';
      break;
  case '1':
      if(mode != 0) c = '1';
      break;
  case '2':
      if(mode != 0) c = '2';
      break;
  case '3':
      if(mode != 0) c = '3';
      break;
  case '4':
      if(mode != 0) c = '4';
      break;
  case '5':
      if(mode != 0) c = '5';
      break;
  case '6':
      if(mode != 0) c = '6';
      break;
  case '7':
      if(mode != 0) c = '7';
      break;
  case '8':
      if(mode != 0) c = '8';
      break;
  case '9':
      if(mode != 0) c = '9';
      break;
  case '.':
      if(mode == 0) c = '.';
      break;
  case ',':
      if(mode == 0) c = ',';
      break;
  case '!':
      if(mode == 0) c = '!';
      break;
  case '?':
      if(mode == 0) c = '?';
      break;
  case ' ':
      if(mode == 0) c = ' ';
      break;
  }

  return c;
}

#endif // KEYIN_H