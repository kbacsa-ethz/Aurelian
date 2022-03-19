#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

GLuint TEXTURE_initialize(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
int TEXTURE_texUnit(GLuint shaderID, const char *uniform, GLuint unit);
int TEXTURE_bind(GLenum texType, GLuint ID);
int TEXTURE_unbind(GLenum texType);
int TEXTURE_delete(GLuint ID);

#endif // TEXTURE_H
