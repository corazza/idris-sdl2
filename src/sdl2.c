#include "sdl2.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>

Mix_Chunk *GAME_loadWav(const char *filepath) {
  return Mix_LoadWAV(filepath);
}

Mix_Music *GAME_loadMusic(const char *filepath) {
  return Mix_LoadMUS(filepath);
}

void GAME_playWav(Mix_Chunk *wav) {
  Mix_PlayChannel(-1, wav, 0);
}

void GAME_playMusic(Mix_Music *music) {
  Mix_PlayMusic(music, -1);
}

void GAME_haltMusic() {
  Mix_HaltMusic();
}

// SDL_AudioDeviceID* deviceId = NULL;
//
// SDL_AudioDeviceID* GAME_openAudioDevice(wav_info *ptr) {
//   deviceId = (SDL_AudioDeviceID *) malloc(sizeof(SDL_AudioDeviceID));
//   *deviceId = SDL_OpenAudioDevice(NULL, 0, &ptr->wavSpec, NULL, 0);
//   return deviceId;
// }
//
// wav_info *GAME_loadWav(const char *filepath) {
//   wav_info *result = (wav_info *) malloc(sizeof(wav_info));
//   SDL_LoadWAV(filepath,
//               &result->wavSpec,
//               &result->wavBuffer,
//               &result->wavLength);
//   if (deviceId == NULL) GAME_openAudioDevice(result);
//   return result;
// }
//
// wav_info *GAME_loadWav(const char *filepath) {
//   wav_info *result = (wav_info *) malloc(sizeof(wav_info));
//   SDL_LoadWAV(filepath,
//               &result->wavSpec,
//               &result->wavBuffer,
//               &result->wavLength);
//   if (deviceId == NULL) GAME_openAudioDevice(result);
//   return result;
// }
//
// void GAME_freeWav(wav_info *ptr) {
//   SDL_FreeWAV(ptr->wavBuffer);
//   free(ptr);
// }
//
// void GAME_closeAudioDevice() {
//   SDL_CloseAudioDevice(*deviceId);
// }
//
// void GAME_playWav(wav_info *ptr) {
//   int success = SDL_QueueAudio(*deviceId, ptr->wavBuffer, ptr->wavLength);
//   SDL_PauseAudioDevice(*deviceId, 0); // unpause
// }

SDL_Renderer* idris_sdl2_init(int width, int height) {
        SDL_SetMainReady();

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |
                     SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
                fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
//return NULL;
                exit(1);
        }

        TTF_Init();

        SDL_Window *window = SDL_CreateWindow(
                "sdl2",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width, height,
                0
                );
        if (window == NULL) {
                printf("Unable to create window: %s\n", SDL_GetError());
// return NULL;
                exit(1);
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(
                window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                );
        if (renderer == NULL) {
                SDL_DestroyWindow(window);
                fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
                exit(1);
        }

        if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
                fprintf(stderr, "SDL_BlendMode failed: %s\n", SDL_GetError());
                exit(1);
        };


        if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ){
            fprintf(stderr, "Mix_OpenAudio failed: %s\n", SDL_GetError());
        }

        return renderer;
}

void quit() {
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}

void filledRect(SDL_Renderer* renderer,
                int x, int y, int w, int h,
                int r, int g, int b, int a)
{
  // printf("rect(%d, %d, %d, %d), rgba(%d, %d, %d, %d)\n", x, y, w, h, r, g, b, a);

        int rc1 = SDL_SetRenderDrawColor(renderer, r, g, b, a);
        if (rc1 != 0) {
                fprintf(stderr, "SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
                exit(1);
        }
        SDL_Rect rect = { x, y, w, h };
        int rc2 = SDL_RenderFillRect(renderer, &rect);
        if (rc2 != 0) {
                fprintf(stderr, "SDL_RenderFillRect failed: %s\n", SDL_GetError());
                exit(1);
        }
}

void outlineRect(SDL_Renderer* renderer,
                 int x, int y, int w, int h,
                 int r, int g, int b, int a)
{
        int rc1 = SDL_SetRenderDrawColor(renderer, r, g, b, a);
        if (rc1 != 0) {
                fprintf(stderr, "SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
                exit(1);
        }
        SDL_Rect rect = { x, y, w, h };
        int rc2 = SDL_RenderDrawRect(renderer, &rect);
        if (rc2 != 0) {
                fprintf(stderr, "SDL_RenderDrawRect failed: %s\n", SDL_GetError());
                exit(1);
        }
}

Sint16 *getPoints(const char *s, int n) {
  int k = 0;
  Sint16 *res = (Sint16*) malloc(sizeof(Sint16) * n);
  char *point = strtok(s, " ");
  do {
      res[k++] = (Sint16) atoi(point);
  } while (point = strtok(NULL, " "));
  return res;
}

void GAME_filledPolygon(SDL_Renderer* renderer, const char *vx_, const char *vy_,
                        int n, int r, int g, int b, int a) {
  // printf("filled alpha: %d\n", a);
  Sint16 *vx = getPoints(vx_, n);
  Sint16 *vy = getPoints(vy_, n);
  filledPolygonRGBA(renderer, vx, vy, n, r, g, b, a);
  free(vx); free(vy);
}

void GAME_outlinePolygon(SDL_Renderer* renderer, const char *vx_, const char *vy_,
                        int n, int r, int g, int b, int a) {
  // printf("outline alpha: %d\n", a);
  Sint16 *vx = getPoints(vx_, n);
  Sint16 *vy = getPoints(vy_, n);
  polygonRGBA(renderer, vx, vy, n, r, g, b, a);
  free(vx); free(vy);
}

void filledEllipse(SDL_Renderer* renderer,
                   int x, int y, int rx, int ry,
                   int r, int g, int b, int a)
{
        filledEllipseRGBA(renderer, x, y, rx, ry, r, g, b, a);
}

void drawLine(SDL_Renderer* renderer,
              int x, int y, int ex, int ey,
              int r, int g, int b, int a)
{
        lineRGBA(renderer, x, y, ex, ey, r, g, b, a);
}

VAL MOTION(VM* vm, int x, int y, int relx, int rely) {
        VAL m;
        idris_constructor(m, vm, 2, 4, 0);
        idris_setConArg(m, 0, MKINT((intptr_t)x));
        idris_setConArg(m, 1, MKINT((intptr_t)y));
        idris_setConArg(m, 2, MKINT((intptr_t)relx));
        idris_setConArg(m, 3, MKINT((intptr_t)rely));
        return m;
}

VAL WHEEL(VM* vm, int x, int y) {
  VAL s;
  idris_constructor(s, vm, 7, 2, 0);
  idris_setConArg(s, 0, MKINT((intptr_t)x));
  idris_setConArg(s, 1, MKINT((intptr_t)y));
  return s;
}

VAL BUTTON(VM* vm, int tag, int b, int x, int y) {
        VAL button;

        switch(b) {
        case SDL_BUTTON_LEFT:
                idris_constructor(button, vm, 0, 0, 0);
                break;
        case SDL_BUTTON_MIDDLE:
                idris_constructor(button, vm, 1, 0, 0);
                break;
        case SDL_BUTTON_RIGHT:
                idris_constructor(button, vm, 2, 0, 0);
                break;
// TODO: The handling of mouse wheel events has changed in SDL2.
/*
    case SDL_BUTTON_WHEELUP:
        idris_constructor(button, vm, 3, 0, 0);
        break;
    case SDL_BUTTON_WHEELDOWN:
        idris_constructor(button, vm, 4, 0, 0);
        break;
 */
        default:
                idris_constructor(button, vm, 0, 0, 0);
                break;
        }

        VAL event;
        idris_constructor(event, vm, tag, 3, 0);
        // idris_setConArg(event, 0, button);
        idris_setConArg(event, 0, MKINT((intptr_t)x));
        idris_setConArg(event, 1, MKINT((intptr_t)y));

        // printf("BUTTON: %d %d\n", x, y);

        return event;
}

VAL RESIZE(VM* vm, int w, int h) {
        VAL m;
        idris_constructor(m, vm, 5, 2, 0);
        idris_setConArg(m, 0, MKINT((intptr_t)w));
        idris_setConArg(m, 1, MKINT((intptr_t)h));
        return m;
}

VAL KEY(VM* vm, int tag, SDL_Keycode key) {
        VAL k;

        // fprintf(stderr, "KEY key = %d\n", key);
        switch(key) {
        case SDLK_UP:
                // fprintf(stderr, "key = UP\n");
                idris_constructor(k, vm, 0, 0, 0);
                break;
        case SDLK_DOWN:
                // fprintf(stderr, "key = DOWN\n");
                idris_constructor(k, vm, 1, 0, 0);
                break;
        case SDLK_LEFT:
                // fprintf(stderr, "key = LEFT\n");
                idris_constructor(k, vm, 2, 0, 0);
                break;
        case SDLK_RIGHT:
                // fprintf(stderr, "key = RIGHT\n");
                idris_constructor(k, vm, 3, 0, 0);
                break;
        case SDLK_ESCAPE:
                // fprintf(stderr, "key = ESCAPE\n");
                idris_constructor(k, vm, 4, 0, 0);
                break;
        case SDLK_SPACE:
                // fprintf(stderr, "key = SPACE\n");
                idris_constructor(k, vm, 5, 0, 0);
                break;
        case SDLK_TAB:
                // fprintf(stderr, "key = TAB\n");
                idris_constructor(k, vm, 6, 0, 0);
                break;
        case SDLK_F1:
                // fprintf(stderr, "key = F1\n");
                idris_constructor(k, vm, 7, 0, 0);
                break;
        case SDLK_F2:
                // fprintf(stderr, "key = F2\n");
                idris_constructor(k, vm, 8, 0, 0);
                break;
        case SDLK_F3:
                // fprintf(stderr, "key = F3\n");
                idris_constructor(k, vm, 9, 0, 0);
                break;
        case SDLK_F4:
                // fprintf(stderr, "key = F4\n");
                idris_constructor(k, vm, 10, 0, 0);
                break;
        case SDLK_F5:
                // fprintf(stderr, "key = F5\n");
                idris_constructor(k, vm, 11, 0, 0);
                break;
        case SDLK_F6:
                // fprintf(stderr, "key = F6\n");
                idris_constructor(k, vm, 12, 0, 0);
                break;
        case SDLK_F7:
                // fprintf(stderr, "key = F7\n");
                idris_constructor(k, vm, 13, 0, 0);
                break;
        case SDLK_F8:
                // fprintf(stderr, "key = F8\n");
                idris_constructor(k, vm, 14, 0, 0);
                break;
        case SDLK_F9:
                // fprintf(stderr, "key = F9\n");
                idris_constructor(k, vm, 15, 0, 0);
                break;
        case SDLK_F10:
                // fprintf(stderr, "key = F10\n");
                idris_constructor(k, vm, 16, 0, 0);
                break;
        case SDLK_F11:
                // fprintf(stderr, "key = F11\n");
                idris_constructor(k, vm, 17, 0, 0);
                break;
        case SDLK_F12:
                // fprintf(stderr, "key = F12\n");
                idris_constructor(k, vm, 18, 0, 0);
                break;
        case SDLK_F13:
                // fprintf(stderr, "key = F13\n");
                idris_constructor(k, vm, 19, 0, 0);
                break;
        case SDLK_F14:
                // fprintf(stderr, "key = F14\n");
                idris_constructor(k, vm, 20, 0, 0);
                break;
        case SDLK_F15:
                // fprintf(stderr, "key = F15\n");
                idris_constructor(k, vm, 21, 0, 0);
                break;
        case SDLK_LSHIFT:
                // fprintf(stderr, "key = LSHIFT\n");
                idris_constructor(k, vm, 22, 0, 0);
                break;
        case SDLK_RSHIFT:
                // fprintf(stderr, "key = RSHIFT\n");
                idris_constructor(k, vm, 23, 0, 0);
                break;
        case SDLK_LCTRL:
                // fprintf(stderr, "key = LCTRL\n");
                idris_constructor(k, vm, 24, 0, 0);
                break;
        case SDLK_RCTRL:
                // fprintf(stderr, "key = RCTRL\n");
                idris_constructor(k, vm, 25, 0, 0);
                break;
        default:
                // fprintf(stderr, "key = OTHER\n");
                idris_constructor(k, vm, 26, 1, 0);
                // safe because there's no further allocation.
                idris_setConArg(k, 0, MKINT((intptr_t)key));
                break;
        }

        VAL event;
        idris_constructor(event, vm, tag, 1, 0);
        idris_setConArg(event, 0, k);

        return event;
}

/*
   data Button = Left | Middle | Right | WheelUp | WheelDown

   data Event = KeyDown Key
 | KeyUp Key
 | MouseMotion Int Int Int Int
 | MouseButtonDown Button Int Int
 | MouseButtonUp Button Int Int
 | AppQuit

   pollEvent : IO (Maybe Event)
 */

void* pollEvent(VM* vm) {
        VAL idris_event;

        SDL_Event event; // = (SDL_Event *) GC_MALLOC(sizeof(SDL_Event));
        int r = SDL_PollEvent(&event);

        idris_requireAlloc(vm, 128); // Conservative!

        if (r==0) {
                idris_constructor(idris_event, vm, 0, 0, 0); // Nothing
        }
        else {
                // fprintf(stderr, "event.type = %d\n", event.type);
                VAL ievent = NULL;
                switch(event.type) {
                case SDL_KEYDOWN:
                        // fprintf(stderr, "KEYDOWN event.key.keysym.sym = %d\n", event.key.keysym.sym);
                        ievent = KEY(vm, 0, event.key.keysym.sym);
                        break;
                case SDL_KEYUP:
                        // fprintf(stderr, "KEYUP   event.key.keysym.sym = %d\n", event.key.keysym.sym);
                        ievent = KEY(vm, 1, event.key.keysym.sym);
                        break;
                case SDL_MOUSEMOTION:
                        ievent = MOTION(vm, event.motion.x, event.motion.y,
                                        event.motion.xrel, event.motion.yrel);
                        break;
                case SDL_MOUSEBUTTONDOWN:
                        ievent = BUTTON(vm, 3, event.button.button,
                                        event.button.x, event.button.y);
                        break;
                case SDL_MOUSEBUTTONUP:
                        ievent = BUTTON(vm, 4, event.button.button,
                                        event.button.x, event.button.y);
                                        // 0, 0);
                        // printf("%d %d\n", event.button.x, event.button.y);
                        break;
                case SDL_MOUSEWHEEL:
                      ievent = WHEEL(vm, event.wheel.x, event.wheel.y);
                      break;
                // case SDL_WINDOWEVENT:
                //         if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                //                 ievent = RESIZE(vm, event.window.data1, event.window.data2);
                //         }
                //         break;

                case SDL_QUIT:
                        // fprintf(stderr, "SDL_QUIT\n");
                        idris_constructor(ievent, vm, 6, 0, 0);
                        break;
                default:
                        idris_constructor(idris_event, vm, 0, 0, 0); // Nothing
                        idris_doneAlloc(vm);
                        return idris_event;
                }
                // fprintf(stderr, "constructing (Just ievent)\n");
                idris_constructor(idris_event, vm, 1, 1, 0);
                idris_setConArg(idris_event, 0, ievent); // Just ievent
        }

        idris_doneAlloc(vm);
        return idris_event;
}

// FIXME: Remove this when events work.
int pollEventsForQuit() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                        return 1;
                        break;
                case SDL_KEYDOWN:
                        if (e.key.keysym.sym == SDLK_ESCAPE) {
                                return 1;
                        }
                        break;
                }
        }
        return 0;
}


// GAME

int GAME_drawTexture(SDL_Renderer* renderer, SDL_Texture* texture,
                     int sx, int sy, int sw, int sh,
                     int dx, int dy, int dw, int dh,
                     int provided) {
        SDL_Rect src = {sx, sy, sw, sh};
        SDL_Rect dst = {dx, dy, dw, dh};

        switch(provided) {
        case 0: return SDL_RenderCopy(renderer, texture, NULL, NULL);
        case 1: return SDL_RenderCopy(renderer, texture, &src, NULL);
        case 2: return SDL_RenderCopy(renderer, texture, NULL, &dst);
        case 3: return SDL_RenderCopy(renderer, texture, &src, &dst);
        }

}

SDL_RendererFlip getFlip(int flip) {
  if (flip == 1) return SDL_FLIP_VERTICAL;
  if (flip == 2) return SDL_FLIP_HORIZONTAL;
  if (flip == 3) return SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
  return SDL_FLIP_NONE;
}

void drawWholeCenter(SDL_Renderer *renderer, SDL_Texture *texture,
                     int dx, int dy, int dw, int dh, int flip, double angle) {
        SDL_Rect dst = {dx, dy, dw, dh};
        SDL_RenderCopyEx(renderer, texture, NULL, &dst, angle, NULL, getFlip(flip));
}

void drawCenter(SDL_Renderer *renderer, SDL_Texture *texture,
                int sx, int sy, int sw, int sh,
                int dx, int dy, int dw, int dh,
                int flip, double angle) {
        SDL_Rect src = {sx, sy, sw, sh};
        SDL_Rect dst = {dx, dy, dw, dh};
        SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, NULL, getFlip(flip));
}

void destroyTexture(SDL_Texture *texture) {
  SDL_DestroyTexture(texture);
}

// void *loadFont()

// TODO FIX URGENT terrible, fonts reopened
void renderText(SDL_Renderer *renderer, const char *text, const char *fontName,
                int size, int r, int g, int b, int a, int x, int y, int w, int h) {
  TTF_Font *font = TTF_OpenFont(fontName, size);

  if (font == NULL) {
    printf("couldn't open font %s\n", fontName);
    return;
  }

  SDL_Color color = {r, g, b};
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dst);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
}
