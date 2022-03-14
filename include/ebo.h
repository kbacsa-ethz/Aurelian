#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

GLuint EBO_initialize(GLuint *indices, GLsizeiptr size);
int EBO_bind(GLuint ID);
int EBO_unbind();
int EBO_delete(GLuint ID);

#endif // EBO_H
