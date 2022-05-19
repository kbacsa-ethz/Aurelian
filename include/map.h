//
// Created by sbeuret on 30.04.22.
//

#ifndef AURELIAN_MAP_H
#define AURELIAN_MAP_H

#include "vbo.h"

// define a custom type for the map amplitude
typedef float MapPos;

// define a structure for a 2d vector in the map
struct MapVec2Struct{
  MapPos x;
  MapPos z;
};
typedef struct MapVec2Struct MapVec2;

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



// get the position in the map associated with indices
MapVec2 MAP_get_index_position(Map map, int x_id, int z_id);

// get the positions array associated to the map and the size of the positions array
GLfloat* MAP_get_positions(Map map);
size_t MAP_get_positions_size(Map map);

// get the indices of the map and the size of the indices array
GLuint* MAP_get_indices(Map map);
size_t MAP_get_indices_size(Map map);

// get the normals to the map and the size of the normals array
GLfloat* MAP_get_normals(Map map);
size_t MAP_get_normals_size(Map map);

// get the uvs
GLfloat* MAP_get_text_uvs(Map map, double uv_max_x, double uv_max_z);
size_t MAP_get_text_uvs_size(Map map);

// get the colors
GLfloat* MAP_get_colors(Map map);
size_t MAP_get_colors_size(Map map);

// free a map
void Map_free(Map* map_ptr);

#endif //AURELIAN_MAP_H
