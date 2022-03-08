#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "statemanager.h"
#include "graphics.h"
#include "engine.h"

// Debuggig macros
#define PRINT_VAR(X) printf(#X " is %d at address %p\n", X, &X);
#define SWAP(A, B) A ^= B ^= A ^= B;

unsigned int initState1() {
  printf("state 1 created\n");
  return 0;
}

unsigned int destroyState1() {
  printf("state 1 destroyed\n");
  return 0;
}

unsigned int initState2() {
  printf("state 2 created\n");
  return 0;
}

unsigned int updateState1(float deltatime) {
  printf("state 1 update %f\n", deltatime);
  return 0;
}

unsigned int updateState2(float deltatime) {
  printf("state 2 update %f\n", deltatime);
  return 0;
}

unsigned int destroyState2() {
  printf("state 2 destroyed\n");
  return 0;
}


int main(int argc, char *argv[])
{
    struct EngineOptions options = {0};
    options.title = "Game Window";
    options.width = 480;
    options.height = 272;

    Engine engine;
    ENGINE_init(&engine, &options);

    // StateManager test code
    //StateManager statemanager;
    //STATEMANAGER_init(&statemanager);

    State state1 = {0};
    state1.init = initState1;
    state1.update = updateState1;;
    state1.destroy = destroyState1;

    State state2 = {0};
    state2.init = initState2;
    state2.update = updateState2;;
    state2.destroy = destroyState2;

    STATEMANAGER_push(&engine.statemanager, &state1);
    STATEMANAGER_update(&engine.statemanager, 10.0f);

    STATEMANAGER_push(&engine.statemanager, &state2);
    STATEMANAGER_update(&engine.statemanager, 10.0f);

    STATEMANAGER_pop(&engine.statemanager);
    STATEMANAGER_update(&engine.statemanager, 10.0f);

    //STATEMANAGER_free(&statemanager);

    // Graphics test code
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
      STATEMANAGER_free(&engine.statemanager);
      return 1;
    }

    SDL_Event e;
    while(!engine.quit) {
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) engine.quit = 1;
      }

      STATEMANAGER_update(&engine.statemanager, 10.0f);
    }

    ENGINE_free(&engine);

    return 0;
}
