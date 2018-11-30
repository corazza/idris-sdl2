#include "sdl2.h"
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>

SDL_Renderer* idris_sdl2_init(int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |
               SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
//return NULL;
    exit(1);
  }

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

  return renderer;
}

void filledRect(SDL_Renderer* renderer,
                int x, int y, int w, int h,
                int r, int g, int b, int a)
{
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
    idris_setConArg(event, 0, button);
    idris_setConArg(event, 1, MKINT((intptr_t)x));
    idris_setConArg(event, 2, MKINT((intptr_t)y));

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

    fprintf(stderr, "KEY key = %d\n", key);
    switch(key) {
    case SDLK_UP:
        fprintf(stderr, "key = UP\n");
        idris_constructor(k, vm, 0, 0, 0);
        break;
    case SDLK_DOWN:
        fprintf(stderr, "key = DOWN\n");
        idris_constructor(k, vm, 1, 0, 0);
        break;
    case SDLK_LEFT:
        fprintf(stderr, "key = LEFT\n");
        idris_constructor(k, vm, 2, 0, 0);
        break;
    case SDLK_RIGHT:
        fprintf(stderr, "key = RIGHT\n");
        idris_constructor(k, vm, 3, 0, 0);
        break;
    case SDLK_ESCAPE:
        fprintf(stderr, "key = ESCAPE\n");
        idris_constructor(k, vm, 4, 0, 0);
        break;
    case SDLK_SPACE:
        fprintf(stderr, "key = SPACE\n");
        idris_constructor(k, vm, 5, 0, 0);
        break;
    case SDLK_TAB:
        fprintf(stderr, "key = TAB\n");
        idris_constructor(k, vm, 6, 0, 0);
        break;
    case SDLK_F1:
        fprintf(stderr, "key = F1\n");
        idris_constructor(k, vm, 7, 0, 0);
        break;
    case SDLK_F2:
        fprintf(stderr, "key = F2\n");
        idris_constructor(k, vm, 8, 0, 0);
        break;
    case SDLK_F3:
        fprintf(stderr, "key = F3\n");
        idris_constructor(k, vm, 9, 0, 0);
        break;
    case SDLK_F4:
        fprintf(stderr, "key = F4\n");
        idris_constructor(k, vm, 10, 0, 0);
        break;
    case SDLK_F5:
        fprintf(stderr, "key = F5\n");
        idris_constructor(k, vm, 11, 0, 0);
        break;
    case SDLK_F6:
        fprintf(stderr, "key = F6\n");
        idris_constructor(k, vm, 12, 0, 0);
        break;
    case SDLK_F7:
        fprintf(stderr, "key = F7\n");
        idris_constructor(k, vm, 13, 0, 0);
        break;
    case SDLK_F8:
        fprintf(stderr, "key = F8\n");
        idris_constructor(k, vm, 14, 0, 0);
        break;
    case SDLK_F9:
        fprintf(stderr, "key = F9\n");
        idris_constructor(k, vm, 15, 0, 0);
        break;
    case SDLK_F10:
        fprintf(stderr, "key = F10\n");
        idris_constructor(k, vm, 16, 0, 0);
        break;
    case SDLK_F11:
        fprintf(stderr, "key = F11\n");
        idris_constructor(k, vm, 17, 0, 0);
        break;
    case SDLK_F12:
        fprintf(stderr, "key = F12\n");
        idris_constructor(k, vm, 18, 0, 0);
        break;
    case SDLK_F13:
        fprintf(stderr, "key = F13\n");
        idris_constructor(k, vm, 19, 0, 0);
        break;
    case SDLK_F14:
        fprintf(stderr, "key = F14\n");
        idris_constructor(k, vm, 20, 0, 0);
        break;
    case SDLK_F15:
        fprintf(stderr, "key = F15\n");
        idris_constructor(k, vm, 21, 0, 0);
        break;
    case SDLK_LSHIFT:
        fprintf(stderr, "key = LSHIFT\n");
        idris_constructor(k, vm, 22, 0, 0);
        break;
    case SDLK_RSHIFT:
        fprintf(stderr, "key = RSHIFT\n");
        idris_constructor(k, vm, 23, 0, 0);
        break;
    case SDLK_LCTRL:
        fprintf(stderr, "key = LCTRL\n");
        idris_constructor(k, vm, 24, 0, 0);
        break;
    case SDLK_RCTRL:
        fprintf(stderr, "key = RCTRL\n");
        idris_constructor(k, vm, 25, 0, 0);
        break;
    default:
        fprintf(stderr, "key = OTHER\n");
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
        fprintf(stderr, "event.type = %d\n", event.type);
        VAL ievent = NULL;
        switch(event.type) {
        case SDL_KEYDOWN:
            fprintf(stderr, "KEYDOWN event.key.keysym.sym = %d\n", event.key.keysym.sym);
            ievent = KEY(vm, 0, event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            fprintf(stderr, "KEYUP   event.key.keysym.sym = %d\n", event.key.keysym.sym);
            ievent = KEY(vm, 1, event.key.keysym.sym);
            break;
/*
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
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
              ievent = RESIZE(vm, event.window.data1, event.window.data2);
            }
            break;
*/
        case SDL_QUIT:
            fprintf(stderr, "SDL_QUIT\n");
            idris_constructor(ievent, vm, 6, 0, 0);
            break;
        default:
            idris_constructor(idris_event, vm, 0, 0, 0); // Nothing
            idris_doneAlloc(vm);
            return idris_event;
        }
        fprintf(stderr, "constructing (Just ievent)\n");
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
