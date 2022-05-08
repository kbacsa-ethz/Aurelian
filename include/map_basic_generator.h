//
// Created by sbeuret on 30.04.22.
//

#ifndef AURELIAN_MAP_BASIC_GENERATOR_H
#define AURELIAN_MAP_BASIC_GENERATOR_H

#include "map.h"

Map MAP_BASIC_GENERATOR_get_sample_map(int size_x, int size_z,
                                       MapPos map_offset_x, MapPos map_offset_z, MapPos cell_length);


#endif //AURELIAN_MAP_BASIC_GENERATOR_H
