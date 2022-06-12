#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <cglm/cglm.h>

# include "mesh_array.h"

GLuint VBO_initialize_Positions_Normals(PositionsArray positions_array, NormalsArray normals_array);
GLuint VBO_initialize_Positions_Normals_TextUVs(PositionsArray positions_array, NormalsArray normals_array,
                                                TextUVsArray textUVs_array);
int VBO_bind(GLuint ID);
int VBO_unbind();
int VBO_delete(GLuint ID);

#endif // VBO_H
