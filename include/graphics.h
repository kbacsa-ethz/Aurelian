#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

typedef struct {
  int width;
  int height;
  char *windowTitle;
} Graphics;

int GRAPHICS_init(Graphics *graphics);
void GRAPHICS_free(Graphics *graphics);
#endif // GRAPHICS_H
