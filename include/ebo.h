#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
#include "mesh_array.h"

GLuint EBO_initialize(IndicesArray indices_array);
int EBO_bind(GLuint ID);
int EBO_unbind();
int EBO_delete(GLuint ID);

#endif // EBO_H
