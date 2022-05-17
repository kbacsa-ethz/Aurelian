//
// Created by sbeuret on 14.05.22.
//

#ifndef AURELIAN_MAP_GRAPHICS_H
#define AURELIAN_MAP_GRAPHICS_H

#include "mesh_array.h"
#include "map.h"
#include "texture.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "camera.h"

// define a structure for the map
struct MapMeshStruct{
    PositionsArray positions_array;
    NormalsArray normals_array;
    TextUVsArray textUVs_array;
    IndicesArray indices_array;

    GLuint *textureIDs_ptr;
    GLuint nTextures;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint shaderID;
};
typedef struct MapMeshStruct MapMesh;


int MAP_GRAPHICS_get_map_mesh(MapMesh* map_mesh, Map map, GLuint shaderID, GLuint *textureIDs, int nTextures,
                              double uv_max_x, double uv_max_z);
int MAP_GRAPHICS_delete_map_mesh(MapMesh *map_mesh_ptr);

int MAP_GRAPHICS_draw_map_mesh(MapMesh *map_mesh_ptr, Camera *camera);


#endif //AURELIAN_MAP_GRAPHICS_H
