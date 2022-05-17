#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <cglm/cglm.h>

# include "mesh_array.h"


GLuint VBO_initialize(PositionsArray positions_array, NormalsArray normals_array,
                      TextUVsArray textUVs_array);
int VBO_bind(GLuint ID);
int VBO_unbind();
int VBO_delete(GLuint ID);

#endif // VBO_H
