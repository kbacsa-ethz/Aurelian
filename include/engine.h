#ifndef ENGINE_H
#define ENGINE_H

#include "graphics.h"
#include "statemanager.h"

struct EngineOptions {
  char *title;
  int width;
  int height;
};

typedef struct {
  StateManager statemanager;
  Graphics graphics;
  char quit;
} Engine;

int ENGINE_init(Engine *engine, struct EngineOptions *options);
int ENGINE_free(Engine *engine);
#endif // ENGINE_H
