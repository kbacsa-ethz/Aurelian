//
// Created by sbeuret on 19.03.22.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "map.h"


MapVec2 get_map_position(Map map, int x_id, int z_id){

  MapPos z_ratio = sqrt(2.)/2;

  // define the local position
  MapPos offset_local = (MapPos)(z_id % 2) * map.cell_length / 2;
  MapPos x_pos = map.offset_x + ((MapPos) x_id) * map.cell_length + offset_local;
  MapPos z_pos = map.offset_z + ((MapPos) z_id) * map.cell_length * z_ratio;

  MapVec2 out_pos = {x_pos, z_pos};
  return out_pos;
}

GLfloat* get_map_vertices(Map map){

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
      MapVec2 local_vec = get_map_position(map, x_id, z_id);

      // fill the vertices id
      vertices_array[3 * (x_id + map.size_x * z_id)] = (GLfloat) local_vec.x;
      vertices_array[3 * (x_id + map.size_x * z_id) + 1] = (GLfloat) map.altitude_array[x_id + map.size_x * z_id];
      vertices_array[3 * (x_id + map.size_x * z_id) + 2] = (GLfloat) local_vec.z;

//      printf("%f %f %f \n", vertices_array[3 * (x_id + map.size_x * z_id)], vertices_array[3 * (x_id + map.size_x * z_id) + 1], vertices_array[3 * (x_id + map.size_x * z_id) + 2]);
    }

  return vertices_array;

}


GLuint* get_map_indices(Map map){

  int size_all = 2 * (map.size_z - 1) * (map.size_z - 1);

  GLuint * indices_array = (GLuint *) malloc(3 * size_all * sizeof(GLuint));
  if(indices_array == NULL){
    printf("Cannot initialize indices \n");
  }

  int local_index_id = 0;

  for(int z_id=0; z_id < map.size_z; z_id++)
    for(int x_id=0; x_id < map.size_x; x_id++){


//      printf("%d , %d : %d  \n", x_id, z_id, (GLuint)(x_id + map.size_x * z_id));

      if(z_id != map.size_z - 1){

//        printf("%d : %d , %d \n", local_index_id, x_id, z_id);
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


void free_map(Map* map_ptr){
  free(map_ptr->altitude_array);
  map_ptr->altitude_array = NULL;
}