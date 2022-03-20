//
// Created by sbeuret on 19.03.22.
//

#ifndef AURELIAN_MAP_H
#define AURELIAN_MAP_H

#include <GLFW/glfw3.h>

// define a custom type for the map amplitude
typedef float MapPos;



// define a structure for the map
struct MapStruct{
  int size_x;
  int size_z;
  MapPos offset_x;
  MapPos offset_z;
  MapPos cell_length;
  MapPos * altitude_array;
};
typedef struct MapStruct Map;

// define a structure for a 2d vector
struct MapVec2Struct{
  MapPos x;
  MapPos z;
};
typedef struct MapVec2Struct MapVec2;

// get the position in the map associated with indices
MapVec2 get_map_position(Map map, int x_id, int z_id);

// get the vertices array associated to the map
GLfloat* get_map_vertices(Map map);

GLuint* get_map_indices(Map map);

// free a map
void free_map(Map* map_ptr);



//float* get_map_vertices(Map map);

//float* get_map_vertices(Map map){
//
//  3 * sizeof(float)
//}

#endif //AURELIAN_MAP_H
