#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

GLuint VAO_initialize();
int VAO_linkVBO(GLuint VBO, GLuint layout);
int VAO_bind(GLuint ID);
int VAO_unbind();
int VAO_delete(GLuint ID);

#endif // VAO_H
