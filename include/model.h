#ifndef MODEL_H
#define MODEL_H

#include <cglm/cglm.h>

#include "cJSON.h"


#include "cJSON.h"
#include <cglm/cglm.h>
#include "mesh.h"
#include "texture.h"


#if 0
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

#endif

typedef struct {
   cJSON *modelJSON;
   unsigned char *data;
   vec3* translation_mesh_array;
   versor* rotation_mesh_array;
   vec3* scale_mesh_array;
   mat4* matrix_mesh_array;
   unsigned int dataLength;
   unsigned int nMeshes;
} Model;


typedef struct {
    unsigned int *vecData;
    unsigned int vecSize;
} intVec;

typedef struct {
    float *vecData;
    unsigned int vecSize;
} floatVec;

int MODEL_initialize(Model *model);
int MODEL_traverseNode(Model *model, int nextNode, mat4 matrix);
int MODEL_delete(Model *model);

#endif
