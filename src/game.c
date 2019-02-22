#include "game.h"
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>

void GAME_drawTexture(SDL_Renderer* renderer, SDL_Texture* texture,
                      int sx, int sy, int sw, int sh,
                      int dx, int dy, int dw, int dh,
                      int provided) {

        switch(provided) {
        case 0:
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                break;

        case 1:
                SDL_Rect src = {sx, sy, sw, sh};
                SDL_RenderCopy(renderer, texture, src, NULL);

        case 2:
                SDL_Rect dst = {dx, dy, dw, dh};
                SDL_RenderCopy(renderer, texture, NULL, dst);

        case 3:
                SDL_Rect src = {sx, sy, sw, sh};
                SDL_Rect dst = {dx, dy, dw, dh};
                SDL_RenderCopy(renderer, texture, src, dst);
        }

}
