#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>

typedef struct {
    GLuint ID;
    char *vertexShaderSource;
    char *fragmentShaderSource;
} Shaders;

int SHADERS_initialize(Shaders *shader, char *vertexFilename, char *fragmentFilename);
int SHADERS_activate(Shaders *shader);
int SHADERS_delete(Shaders *shader);

#endif // SHADERS_H
