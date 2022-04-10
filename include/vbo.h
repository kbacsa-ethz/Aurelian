#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct {
    vec2 textUV;
    vec3 position;
    vec3 color;
    vec3 normal;
} Vertex;


GLuint VBO_initialize(Vertex *vertices);
int VBO_bind(GLuint ID);
int VBO_unbind();
int VBO_delete(GLuint ID);

#endif // VBO_H
