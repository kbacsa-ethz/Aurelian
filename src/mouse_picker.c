//
// Created by sbeuret on 23.04.22.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mouse_picker.h"

void MOUSE_PICKER_get_3D_cursor_position(double mouse_x, double mouse_y, mat4 view_mat, mat4 projection_mat,
                                         unsigned int width_display, unsigned int height_display, vec3 world_coord_3d_out){

  // compute the coordinates in the normalized coordinates of the screen
  float x = (2.f * ((float) mouse_x)) / ((float) width_display) - 1.f;
  float y = (2.f * ((float) mouse_y)) / ((float) height_display) - 1.f;

  // get a 4d vector for the clipped coordinates
  vec4 clipped_coord = {x, y, -1.f, 1.f};

  // invert the projection matrix
  mat4 inv_projection_mat;
  glm_mat4_inv(projection_mat, inv_projection_mat);
  vec4 eye_coord;
  glm_mat4_mulv(inv_projection_mat, clipped_coord, eye_coord);

  // invert the view matrix
  mat4 inv_view_mat;
  glm_mat4_inv(view_mat, inv_view_mat);
  vec4 world_coord;
  glm_mat4_mulv(inv_view_mat, eye_coord, world_coord);

  // get a 3d vector and normalize the vector
  world_coord_3d_out[0] = world_coord[0];
  world_coord_3d_out[1] = world_coord[1];
  world_coord_3d_out[2] = world_coord[2];

  glm_vec3_normalize(world_coord_3d_out);

}