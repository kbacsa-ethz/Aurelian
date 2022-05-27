//
// Created by sbeuret on 27.05.22.
//

#ifndef AURELIAN_MODEL_H
#define AURELIAN_MODEL_H

#include "cJSON.h"
#include <cglm/cglm.h>
#include "mesh.h"
#include "texture.h"

typedef struct {

    int n_mesh;
    Mesh* mesh_array;

    int n_texture;
    GLuint* tex_array;

    vec3* translation_mesh_array;
    versor* rotation_mesh_array;
    vec3* scale_mesh_array;
    mat4* matrix_mesh_array;

    GLuint shaderID;
} Model;

int MODEL_initialize_json(Model* model_ptr, char* file);
int MODEL_delete(Model* model_ptr);



#endif //AURELIAN_MODEL_H
