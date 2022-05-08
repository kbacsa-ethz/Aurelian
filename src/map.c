//
// Created by sbeuret on 30.04.22.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "map.h"

// === Internal function ===

// get the position associated with an index
MapVec2 MAP_get_index_position(Map map, int x_id, int z_id){

    MapPos z_ratio = (MapPos) (sqrt(2.) / 2.);

    // define the local position
    MapPos offset_local = (MapPos)(z_id % 2) * map.cell_length / 2;
    MapPos x_pos = map.offset_x + ((MapPos) x_id) * map.cell_length + offset_local;
    MapPos z_pos = map.offset_z + ((MapPos) z_id) * map.cell_length * z_ratio;

    MapVec2 out_pos = {x_pos, z_pos};
    return out_pos;
}





// === Get the positions of the array ===

// get the positions associated with the map
GLfloat* MAP_get_positions(Map map){

    // compute the full size of the array
    int size_all = map.size_x * map.size_z;

    // allocate the array
    GLfloat * vertices_array = (GLfloat *) malloc(3 * size_all * sizeof(GLfloat));
    if(vertices_array == NULL){
        printf("Cannot initialize vertices \n");
    }

    // fill in the map
    for(int z_id=0; z_id < map.size_z; z_id++)
        for(int x_id=0; x_id < map.size_x; x_id++) {

            // get the position of the id
            MapVec2 local_vec = MAP_get_index_position(map, x_id, z_id);

            // fill the vertices id
            vertices_array[3 * (x_id + map.size_x * z_id)] = (GLfloat) local_vec.x;
            vertices_array[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) map.altitude_array[x_id + map.size_x * z_id];
            vertices_array[3 * (x_id + map.size_x * z_id) + 2] = (GLfloat) local_vec.z;

        }
    return vertices_array;
}

// get the size of positions
size_t MAP_get_positions_size(Map map){
    return 3 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);
}


// === Indices management ===

// get the indices of the map
GLuint* MAP_get_indices(Map map){

    int size_all = 2 * (map.size_z - 1) * (map.size_z - 1);

    GLuint * indices_array = (GLuint *) malloc(3 * size_all * sizeof(GLuint));
    if(indices_array == NULL){
        printf("Cannot initialize indices \n");
    }

    int local_index_id = 0;

    for(int z_id=0; z_id < map.size_z; z_id++)
        for(int x_id=0; x_id < map.size_x; x_id++){

            if(z_id != map.size_z - 1){

                if(z_id % 2 == 0){
                    if(x_id==0){
                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                    else if(x_id == map.size_x - 1){
                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id - 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                    else{
                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id - 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;

                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                }
                else{
                    if(x_id != map.size_x - 1){
                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * (z_id + 1));
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id + map.size_x * (z_id + 1));
                        local_index_id += 1;

                        indices_array[3 * local_index_id] = (GLuint) (x_id + map.size_x * z_id);
                        indices_array[3 * local_index_id + 1] = (GLuint) (x_id + 1 + map.size_x * z_id);
                        indices_array[3 * local_index_id + 2] = (GLuint) (x_id + 1 + map.size_x * (z_id + 1));
                        local_index_id += 1;
                    }
                }
            }
        }
    return indices_array;
}

// get the size of the indices
size_t MAP_get_indices_size(Map map){
    return (3 * 2 * (((size_t) map.size_x) - 1) * (((size_t) map.size_z) - 1) * sizeof(GLint));
}

// === Normals management ===

// get the normals array
GLfloat* MAP_get_normals(Map map){

    int size_all = map.size_x * map.size_z;

    // allocate the array
    GLfloat * normals_array = (GLfloat *) malloc(3 * size_all * sizeof(GLfloat));
    if(normals_array == NULL){
        printf("Cannot initialize vertices \n");
    }

    MapPos sqrt_3 = sqrt(3.);

    MapPos norm_grad_x = (6 * map.cell_length);
    MapPos norm_grad_z = (2 * sqrt(3.) * map.cell_length);

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
            normals_array[3 * (x_id + map.size_x * z_id)] = -(GLfloat) grad_x;
            normals_array[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) 1.;
            normals_array[3 * (x_id + map.size_x * z_id) + 2] = -(GLfloat) grad_z;
        }

    return normals_array;
}

// get the size of the normals array
size_t MAP_get_normals_size(Map map) {
    return 3 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);
}

// === UVs (probably temporary) ===

// get the UVs
GLfloat *MAP_get_text_uvs(Map map, double uv_max_x, double uv_max_z) {

    // compute the full size of the array
    int size_all = map.size_x * map.size_z;

    // allocate the array
    GLfloat *uvs_array = (GLfloat *) malloc(2 * size_all * sizeof(GLfloat));
    if (uvs_array == NULL) {
        printf("Cannot initialize vertices \n");
    }

    // fill in the map
    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            MapVec2 local_vec = MAP_get_index_position(map, x_id, z_id);

            double uv_x = local_vec.x / uv_max_x;
            double uv_z = local_vec.z / uv_max_z;

            uvs_array[2 * (x_id + map.size_x * z_id)] = (GLfloat) uv_x;
            uvs_array[2 * (x_id + map.size_x * z_id) + 1] = (GLfloat) uv_z;
        }

    return uvs_array;
}

// get the size of the UV map
size_t MAP_get_text_uvs_size(Map map) {
    return 2 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);
}

// === colors (probably temporary) ===

// get the colors
GLfloat *MAP_get_colors(Map map) {

    // compute the full size of the array
    int size_all = map.size_x * map.size_z;

    // allocate the array
    GLfloat *colors_array = (GLfloat *) malloc(3 * size_all * sizeof(GLfloat));
    if (colors_array == NULL) {
        printf("Cannot initialize vertices \n");
    }

    for (int z_id = 0; z_id < map.size_z; z_id++)
        for (int x_id = 0; x_id < map.size_x; x_id++) {

            colors_array[3 * (x_id + map.size_x * z_id)] = (GLfloat) 0.1;
            colors_array[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) 0.2;
            colors_array[3 * (x_id + map.size_x * z_id) + 2] = (GLfloat) 0.3;
        }

    return colors_array;
}

// get the size of the normals array
size_t MAP_get_colors_size(Map map) {
    return 3 * ((size_t) map.size_x) * ((size_t) map.size_z) * sizeof(GLfloat);
}


// free a map
void Map_free(Map *map_ptr) {
    free(map_ptr->altitude_array);
    map_ptr->altitude_array = NULL;
}
