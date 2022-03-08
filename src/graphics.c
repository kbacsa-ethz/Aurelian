#include <stdlib.h>

//#include "SDL.h"
#include "graphics.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

int GRAPHICS_init(Graphics *graphics) {
#if 0
    if (!graphics -> windowTitle) graphics -> windowTitle = "No Title";
    if (!graphics -> width) graphics -> width = WINDOW_WIDTH;
    if (!graphics -> height) graphics -> height = WINDOW_HEIGHT;
    graphics -> window = SDL_CreateWindow(
                graphics -> windowTitle,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                graphics -> width,
                graphics -> height,
                SDL_WINDOW_SHOWN
                );

    if (graphics -> window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }
#endif

    return 0;
}

void GRAPHICS_free(Graphics *graphics) {
    //SDL_DestroyWindow(graphics -> window);
}
