#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

GLuint VBO_initialize(GLfloat *vertices, GLsizeiptr size);
int VBO_bind(GLuint ID);
int VBO_unbind();
int VBO_delete(GLuint ID);

#endif // VBO_H
