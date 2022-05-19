//
// Created by sbeuret on 14.05.22.
//

//#include "map.h"
#include "map_graphics.h"
//#include "mesh_array.h"

// TODO: add the free of the EBO VBO VAO

//////////////// Functions to fill the different arrays ////////////////


int MAP_GRAPHICS_compute_positions(PositionsArray *positions_array_ptr, Map map) {

    // determine the size of the positions
    positions_array_ptr->size_positions = 3 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);

    // allocate the array
    GLfloat *positions_ptr = (GLfloat *) malloc(positions_array_ptr->size_positions);
    if (positions_ptr == NULL) {
        printf("Cannot initialize poistions \n");
        return -1;
    }

    positions_array_ptr->positions_ptr = positions_ptr;

    // fill in the map
    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            // get the position of the id
            MapVec2 local_vec = MAP_get_index_position(map, x_id, z_id);

            // fill the vertices id
            positions_ptr[3 * (x_id + map.size_x * z_id)] = (GLfloat) local_vec.x;
            positions_ptr[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) map.altitude_array[x_id + map.size_x * z_id];
            positions_ptr[3 * (x_id + map.size_x * z_id) + 2] = (GLfloat) local_vec.z;

        }

    return 0;
}

// get the normals of the map
int MAP_GRAPHICS_compute_normals(NormalsArray *normals_array_ptr, Map map) {

    // determine the size of the positions
    normals_array_ptr->size_normals = 3 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);

    // allocate the array
    GLfloat *normals_ptr = (GLfloat *) malloc(normals_array_ptr->size_normals);
    if (normals_ptr == NULL) {
        printf("Cannot initialize normals \n");
        return -1;
    }

    normals_array_ptr->normals_ptr = normals_ptr;

    MapPos sqrt_3 = (MapPos) sqrt(3.);

    MapPos norm_grad_x = (6 * map.cell_length);
    MapPos norm_grad_z = (2 * (MapPos) sqrt(3.) * map.cell_length);

    // fill in the map
    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            MapPos val_left;
            MapPos val_right;

            // get the values on the same line
            if (x_id > 0) {
                val_left = map.altitude_array[x_id - 1 + map.size_x * z_id];
            } else {
                val_left = map.altitude_array[x_id + map.size_x * z_id];
            }

            if (x_id < map.size_x - 1) {
                val_right = map.altitude_array[x_id + 1 + map.size_x * z_id];
            } else {
                val_right = map.altitude_array[x_id + map.size_x * z_id];
            }

            // define the local variables
            MapPos val_left_up;
            MapPos val_left_down;
            MapPos val_right_up;
            MapPos val_right_down;

            // get the values on the other lines
            if (z_id % 2 == 0) {

                if (x_id > 0 && z_id > 0) {
                    val_left_up = map.altitude_array[x_id - 1 + map.size_x * (z_id - 1)];
                } else {
                    val_left_up = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (x_id > 0 && z_id < map.size_z - 1) {
                    val_left_down = map.altitude_array[x_id - 1 + map.size_x * (z_id + 1)];
                } else {
                    val_left_down = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (z_id > 0) {
                    val_right_up = map.altitude_array[x_id + map.size_x * (z_id - 1)];
                } else {
                    val_right_up = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (z_id < map.size_z - 1) {
                    val_right_down = map.altitude_array[x_id + map.size_x * (z_id + 1)];
                } else {
                    val_right_down = map.altitude_array[x_id + map.size_x * z_id];
                }

            } else {

                if (z_id > 0) {
                    val_left_up = map.altitude_array[x_id + map.size_x * (z_id - 1)];
                } else {
                    val_left_up = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (z_id < map.size_z - 1) {
                    val_left_down = map.altitude_array[x_id + map.size_x * (z_id + 1)];
                } else {
                    val_left_down = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (x_id < map.size_x - 1 && z_id > 0) {
                    val_right_up = map.altitude_array[x_id + 1 + map.size_x * (z_id - 1)];
                } else {
                    val_right_up = map.altitude_array[x_id + map.size_x * z_id];
                }

                if (x_id < map.size_x - 1 && z_id < map.size_z - 1) {
                    val_right_down = map.altitude_array[x_id + 1 + map.size_x * (z_id + 1)];
                } else {
                    val_right_down = map.altitude_array[x_id + map.size_x * z_id];
                }
            }

            // compute the gradient
            MapPos grad_x = ((2 * val_right + val_right_up + val_right_down) -
                             (2 * val_left + val_left_up + val_left_down)) / norm_grad_x;
            MapPos grad_z = ((val_right_down + val_left_down) - (val_left_up + val_right_up)) / norm_grad_z;


            // fill the vertices id
            normals_ptr[3 * (x_id + map.size_x * z_id)] = -(GLfloat) grad_x;
            normals_ptr[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) 1.;
            normals_ptr[3 * (x_id + map.size_x * z_id) + 2] = -(GLfloat) grad_z;
        }

    return 0;
}


// get the text UV map
int MAP_GRAPHICS_compute_textUVs(TextUVsArray *textUVs_array_ptr, Map map, double uv_max_x, double uv_max_z) {

    // compute the full size of the array
    textUVs_array_ptr->size_textUVs = 2 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);

    // allocate the array
    GLfloat *textUVs_ptr = (GLfloat *) malloc(textUVs_array_ptr->size_textUVs);
    if (textUVs_ptr == NULL) {
        printf("Cannot initialize normals \n");
        return -1;
    }

    textUVs_array_ptr->textUVs_ptr = textUVs_ptr;

    // fill in the map
    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            MapVec2 local_vec = MAP_get_index_position(map, x_id, z_id);

            double uv_x = local_vec.x / uv_max_x;
            double uv_z = local_vec.z / uv_max_z;

            textUVs_ptr[2 * (x_id + map.size_x * z_id)] = (GLfloat) uv_x;
            textUVs_ptr[2 * (x_id + map.size_x * z_id) + 1] = (GLfloat) uv_z;
        }

    return 0;

}

// get the indices of the map
int MAP_GRAPHICS_compute_indices(IndicesArray *indices_array_ptr, Map map) {

    indices_array_ptr->size_indices = (3 * 2 * (((size_t) map.size_x) - 1) *
                                       (((size_t) map.size_z) - 1) * sizeof(GLuint));

    // allocate the array
    GLuint *indices_ptr = (GLuint *) malloc(indices_array_ptr->size_indices);
    if (indices_ptr == NULL) {
        printf("Cannot initialize indices \n");
        return -1;
    }

    indices_array_ptr->indices_ptr = indices_ptr;

    // fill the array
    int local_index_id = 0;

    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            if (z_id != map.size_z - 1) {

                if (z_id % 2 == 0) {
                    if (x_id == 0) {
                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    } else if (x_id == map.size_x - 1) {
                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id - 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    } else {
                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id - 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;

                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                } else {
                    if (x_id != map.size_x - 1) {
                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * (z_id + 1));
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;

                        indices_ptr[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_ptr[3 * local_index_id + 2] = (GLuint) (x_id + 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                }
            }
        }
    return 0;
}


//////////////// Functions to handle the mesh ////////////////

int MAP_GRAPHICS_get_map_mesh(MapMesh *map_mesh, Map map, GLuint shaderID, GLuint *textureIDs, int nTextures,
                              double uv_max_x, double uv_max_z) {

    // initialize the mesh
    MAP_GRAPHICS_compute_positions(&(map_mesh->positions_array), map);
    MAP_GRAPHICS_compute_normals(&(map_mesh->normals_array), map);
    MAP_GRAPHICS_compute_textUVs(&(map_mesh->textUVs_array), map, uv_max_x, uv_max_z);
    MAP_GRAPHICS_compute_indices(&(map_mesh->indices_array), map);

    // bind with the buffers

    // Create vertex array object reference (allows to switch between VBOs)
    map_mesh->VAO = VAO_initialize();
    VAO_bind(map_mesh->VAO);

    // Create buffer object reference
    map_mesh -> VBO = VBO_initialize(map_mesh->positions_array, map_mesh->normals_array,
                                     map_mesh->textUVs_array);

    // Create index buffer object reference
    map_mesh -> EBO = EBO_initialize(map_mesh->indices_array);

    // Links VBO to VAO
    VAO_linkAttrib(map_mesh -> VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);

    // Normals
    VAO_linkAttrib(map_mesh -> VBO, 1, 3, GL_FLOAT, 3 * sizeof(float),
                   (void *)((map_mesh -> positions_array).size_positions));
    // Texture coordinates
    VAO_linkAttrib(map_mesh -> VBO, 2, 2, GL_FLOAT, 2 * sizeof(float),
                   (void *)(((map_mesh -> positions_array).size_positions) +
                       ((map_mesh -> normals_array).size_normals)));

    VAO_unbind();
    VBO_unbind();
    EBO_unbind();

    // set the texture
    map_mesh->shaderID = shaderID;
    map_mesh->textureIDs_ptr = textureIDs;
    map_mesh->nTextures = nTextures;

    return 0;

}

// Functions to create the mesh

int MAP_GRAPHICS_delete_map_mesh(MapMesh *map_mesh_ptr) {
    // free the arrays
    POSITIONS_ARRAY_free(&(map_mesh_ptr->positions_array));
    NORMALS_ARRAY_free(&(map_mesh_ptr->normals_array));
    TEXTUVS_ARRAY_free(&(map_mesh_ptr->textUVs_array));
    INDICES_ARRAY_free(&(map_mesh_ptr->indices_array));

    //free the buffers
    EBO_delete(map_mesh_ptr->EBO);
    VBO_delete(map_mesh_ptr->VBO);
    VAO_delete(map_mesh_ptr->VAO);

    //free the map mesh
    free(map_mesh_ptr);

    return 0;
    //
}

int MAP_GRAPHICS_draw_map_mesh(MapMesh *map_mesh_ptr, Camera *camera){

    VAO_bind(map_mesh_ptr -> VAO);

        for (unsigned int i = 0; i < map_mesh_ptr -> nTextures; i++) {
        // Activate uniform (mapping function)
        char string[4];
        snprintf(string, 4, "tex%d", i);
        TEXTURE_texUnit(map_mesh_ptr -> shaderID, "tex0", i);
        TEXTURE_bind(GL_TEXTURE_2D, *(map_mesh_ptr -> textureIDs_ptr+i)); // bind to texture reference
    }

        // Camera orientation and projection
    CAMERA_matrix(camera, map_mesh_ptr -> shaderID, "camMatrix");

    // Actual draw function
    // Primitive, index, number of vertices
    glDrawElements(GL_TRIANGLES, (map_mesh_ptr -> indices_array).size_indices / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    return 0;
}

//SHADERS_activate(shaderID);
//    VAO_bind(mesh -> VAO);
//
//    for (unsigned int i = 0; i < mesh -> nTextures; i++) {
//        // Activate uniform (mapping function)
//        char string[4];
//        snprintf(string, 4, "tex%d", i);
//        TEXTURE_texUnit(shaderID, "tex0", i);
//        TEXTURE_bind(GL_TEXTURE_2D, *(mesh -> textureIDs+i)); // bind to texture reference
//    }
//
//    // Camera orientation and projection
//    CAMERA_matrix(camera, shaderID, "camMatrix");
//
//    // Actual draw function
//    // Primitive, index, number of vertices
//    glDrawElements(GL_TRIANGLES, mesh -> indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);
//    return 0;