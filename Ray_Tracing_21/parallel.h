#ifndef PARALLEL_H
#define PARALLEL_H

#include "rtweekend.h"


#include <iostream>
#include <mpi.h>
#include "camera_parallel.h"
#include "hittable_list.h"
#include "color_array.h"

class parallel
{
public:
    parallel(camera_parallel* _cam, hittable_list* _world);

    color_array* const color_array_ptr();
    color_array* const color_array_all_ptr();
    void render();
    void gather();
    int return_rank() const;

protected:
    int size, rank;
    int width_per_node, height_per_node;
    int width_min, width_max, height_min, height_max;
    camera_parallel* cam;
    hittable_list* world;
    MPI_Comm MPI_world;
    color_array c_array, c_array_all;
};

#endif
