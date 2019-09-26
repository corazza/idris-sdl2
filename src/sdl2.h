#define SDL_MAIN_HANDLED
#ifndef SDL2_H__
#define SDL2_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <idris_rts.h>

SDL_Renderer* idris_sdl2_init(int width, int height);
void quit();


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

void renderText(SDL_Renderer *renderer, const char *text, const char *fontName, int size,
                int r, int g, int b, int a, int x, int y, int w, int h);

// Events

void* pollEvent(VM* vm); // builds an Idris value
int pollEventsForQuit();


// GAME

Mix_Chunk *GAME_loadWav(const char *filepath);
Mix_Music *GAME_loadMusic(const char *filepath);
void GAME_playWav(Mix_Chunk *wav);

void GAME_playMusic(Mix_Music *music);

void GAME_haltMusic();

// typedef struct {
//   SDL_AudioSpec wavSpec;
//   Uint32 wavLength;
//   Uint8 *wavBuffer;
// } wav_info;
//
// wav_info *GAME_loadWav(const char *filepath);
// void GAME_freeWav(wav_info *ptr);
// void GAME_playWav(wav_info *ptr);

// to avoid managing SDL_Rects
int GAME_drawTexture(SDL_Renderer* renderer, SDL_Texture* texture,
                     int dx, int dy, int dw, int dh,
                     int sx, int sy, int sw, int sh,
                     int provided);

void drawWholeCenter(SDL_Renderer *renderer, SDL_Texture *texture,
                     int dx, int dy, int dw, int dh, int flip, double angle);

void drawCenter(SDL_Renderer *renderer, SDL_Texture *texture,
                int sx, int sy, int sw, int sh,
                int dx, int dy, int dw, int dh,
                int flip, double angle);


void destroyTexture(SDL_Texture *texture);

#endif
