#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "ebo.h"
#include "camera.h"
#include "texture.h"

typedef struct {
    GLuint VAO;
    GLuint *indices;
    GLfloat *vertices;
    GLfloat *textures;
} Mesh;

#endif
