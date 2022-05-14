//
// Created by sbeuret on 14.05.22.
//

#ifndef AURELIAN_MESH_ARRAY_H
#define AURELIAN_MESH_ARRAY_H

// positions array
typedef struct {
  GLfloat *positions_ptr;
  size_t size_positions;
} PositionsArray;

// normals array
typedef struct {
  GLfloat *normals_ptr;
  size_t size_normals;
} NormalsArray;

// textUVs array
typedef struct {
  GLfloat *textUVs_ptr;
  size_t size_textUVs;
} TextUVsArray;

// indices array
typedef struct {
  GLuint *indices_ptr;
  size_t size_indices;
} IndicesArray;

// functions to free the arrays
void POSITIONS_ARRAY_free(PositionsArray* positions_array_ptr);
void NORMALS_ARRAY_free(NormalsArray* normals_array_ptr);
void TEXTUVS_ARRAY_free(TextUVsArray* textUVs_array_ptr);
void INDICES_ARRAY_free(IndicesArray* indices_array_ptr);


#endif //AURELIAN_MESH_ARRAY_H
