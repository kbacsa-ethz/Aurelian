#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>

int SHADERS_initialize(char *vertexFilename, char *fragmentFilename);
int SHADERS_activate(GLuint ID);
int SHADERS_delete(GLuint ID);

#endif // SHADERS_H
