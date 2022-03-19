#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>

GLuint SHADERS_initialize(char *vertexFilename, char *fragmentFilename);
int SHADERS_activate(GLuint ID);
int SHADERS_delete(GLuint ID);
int SHADERS_compileErrors(unsigned int shader, const char *type);

#endif // SHADERS_H
