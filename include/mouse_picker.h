//
// Created by sbeuret on 23.04.22.
//

#ifndef AURELIAN_MOUSE_PICKER_H
#define AURELIAN_MOUSE_PICKER_H

#include <cglm/cglm.h>

void MOUSE_PICKER_get_3D_cursor_position(double mouse_x, double mouse_y, mat4 view_mat, mat4 projection_mat,
                                         unsigned int width_display, unsigned int height_display, vec3 world_coord_3d_out);

#endif //AURELIAN_MOUSE_PICKER_H
