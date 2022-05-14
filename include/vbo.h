#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <cglm/cglm.h>

// reformat to DoD

typedef struct {
    GLfloat *positions;
    GLfloat *normals;
    GLfloat *textUVs;
    size_t sizePositions;
    size_t sizeNormals;
    size_t sizeTextUVs;
} Vertices;


GLuint VBO_initialize(Vertices *vertices);
int VBO_bind(GLuint ID);
int VBO_unbind();
int VBO_delete(GLuint ID);

#endif // VBO_H
