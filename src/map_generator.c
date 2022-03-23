//
// Created by sbeuret on 19.03.22.
//

#include "map_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Map get_sample_map(int size_x, int size_z, MapPos map_offset_x, MapPos map_offset_z, MapPos cell_length){

  // compute the full size of the array
  int size_all = size_x * size_z;

  // allocate the memory for the map
  MapPos * map_array = (MapPos *) malloc(3 * size_all * sizeof(MapPos));
  if(map_array == NULL){
    printf("Cannot initialize map \n");
  }

  printf("Hello \n" );

  // define the out map
  Map map_out = {size_x, size_z, map_offset_x, map_offset_z, cell_length, map_array};

  for(int z_id=0; z_id < size_z; z_id++)
    for(int x_id=0; x_id < size_x; x_id++){

      MapVec2 local_vec_pos = get_map_position(map_out, x_id, z_id);

      // compute the map
      double radius_squared = (double) (local_vec_pos.x * local_vec_pos.x + local_vec_pos.z * local_vec_pos.z);
      map_array[x_id  + z_id * size_x] = (MapPos) (0.1 * sin(M_PI * radius_squared));
    }

  return map_out;
}
