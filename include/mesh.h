#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "texture.h"
#include "camera.h"

typedef struct {
    PositionsArray positions_array;
    NormalsArray normals_array;
    TextUVsArray textUVs_array;
    IndicesArray indices_array;

    GLuint *textureIDs_ptr;
    GLuint nTextures;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint shaderID;
} Mesh;

int MESH_initialize(Mesh *mesh, PositionsArray positions_array, NormalsArray normals_array, TextUVsArray textUVs_array,
                    IndicesArray indices_array, GLuint *textureIDs_ptr, int nTextures, GLuint shaderID);
int MESH_draw(Mesh *mesh, Camera *camera);
int MESH_delete(Mesh *mesh);

#endif
