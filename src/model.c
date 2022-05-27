//
// Created by sbeuret on 27.05.22.
//

#include "model.h"

int MODEL_get_n_texture(const cJSON* json_main_ptr, int* n_texture_ptr){

    *n_texture_ptr = 1;
    return 0;

    // return error
    return -1;

}

int MODEL_get_n_mesh(const cJSON* json_main_ptr, int* n_mesh_ptr){

    *n_mesh_ptr = 1;
    return 0;

    // return error
    // return -1;
}

int MODEL_fill_json(const cJSON* json_main_ptr, Mesh* mesh_array, GLuint* texture_array, vec3* translation_mesh_array,
                    versor* rotation_mesh_array, vec3* scale_mesh_array, mat4* matrix_mesh_array){

    // return error
    return -1;

}

int MODEL_initialize_json(Model* model_ptr, char* file){

    cJSON* json_main_ptr = cJSON_Parse(file);
    if(json_main_ptr == NULL){
        fprintf(stderr, "Failed to parse the file %s", file);
        return -1;
    }

    // first determine the number of textures and meshes
    int n_tex=0;
    if(MODEL_get_n_texture(json_main_ptr, &n_tex)){
        fprintf(stderr, "Failed to determine the number of textures while parsing %s", file);
        return -1;
    }

    int n_mesh=0;
    if(MODEL_get_n_mesh(json_main_ptr, &n_mesh)){
        fprintf(stderr, "Failed to determine the number of mesh while parsing %s", file);
        return -1;
    }

    // allocate the required memory
    model_ptr->mesh_array = (Mesh*) malloc(n_mesh * sizeof(Mesh));
    if(model_ptr->mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the meshes in the parsing of %s", file);
        return -1;
    }

    model_ptr->tex_array = (GLuint*) malloc(n_tex * sizeof(Mesh));
    if(model_ptr->mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the textures in the parsing of %s", file);
        return -1;
    }

    // allocate the transformation arrays
    model_ptr->translation_mesh_array = (vec3*) malloc(n_mesh * sizeof(vec3));
    if(model_ptr->translation_mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the translations in the parsing of %s", file);
        return -1;
    }

    model_ptr->rotation_mesh_array = (versor*) malloc(n_mesh * sizeof(versor));
    if(model_ptr->rotation_mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the rotations in the parsing of %s", file);
        return -1;
    }

    model_ptr->scale_mesh_array = (vec3*) malloc(n_mesh * sizeof(vec3));
    if(model_ptr->scale_mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the scales in the parsing of %s", file);
        return -1;
    }

    model_ptr->matrix_mesh_array = (mat4*) malloc(n_mesh * sizeof(mat4));
    if(model_ptr->matrix_mesh_array == NULL){
        fprintf(stderr, "Failed to allocate memory for the scales in the parsing of %s", file);
        return -1;
    }

    // fill the model
    if(MODEL_fill_json(json_main_ptr, model_ptr->mesh_array, model_ptr->tex_array,
                       model_ptr->translation_mesh_array, model_ptr->rotation_mesh_array,
                       model_ptr->scale_mesh_array, model_ptr->matrix_mesh_array)){
        fprintf(stderr, "Failed to determine the number of textures while parsing %s", file);
        return -1;
    }

    return 0;

}
int MODEL_delete(Model* model_ptr){

    // free the transformation arrays
    free(model_ptr->translation_mesh_array);
    free(model_ptr->rotation_mesh_array);
    free(model_ptr->scale_mesh_array);
    free(model_ptr->matrix_mesh_array);

    //free the meshes
    int mesh_id;
    for(mesh_id=0; mesh_id < model_ptr->n_mesh; mesh_id++){
        MESH_delete((model_ptr->mesh_array + mesh_id));
    }
    free(model_ptr->mesh_array);
    model_ptr->mesh_array = NULL;

    //free the textures
    int tex_id;
    for(tex_id=0; tex_id < model_ptr->n_texture; tex_id++){
        TEXTURE_delete(*(model_ptr->tex_array + tex_id));
    }
    free(model_ptr->tex_array);
    model_ptr->tex_array = NULL;

    return 0;
}



