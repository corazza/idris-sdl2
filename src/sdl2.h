#ifndef SDL2_H__
#define SDL2_H__

#include <SDL.h>
#include <idris_rts.h>

SDL_Renderer* idris_sdl2_init(int width, int height);

// Drawing primitives

void filledRect(SDL_Renderer *renderer,
                int x, int y, int w, int h,
                int r, int g, int b, int a);

void filledEllipse(SDL_Renderer *renderer,
                   int x, int y, int rx, int ry,
                   int r, int g, int b, int a);

void drawLine(SDL_Renderer *renderer,
              int x, int y, int ex, int ey,
              int r, int g, int b, int a);

// Events

void* pollEvent(VM* vm); // builds an Idris value
int pollEventsForQuit();

#endif
