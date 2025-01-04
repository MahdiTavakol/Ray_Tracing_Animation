#ifndef PARALLEL_H
#define PARALLEL_H

#include "rtweekend.h"


#include <iostream>
#include <mpi.h>
#include "camera_parallel.h"
#include "camera_derived.h"
#include "hittable_list.h"
#include "hittable_list_parallel.h"
#include "color_array.h"


class parallel
{
public:
    parallel(hittable_list* _world, camera_parallel* _cam);
    parallel(hittable_list_parallel* _world_parallel, camera_derived* _cam_derived);
    void MPI_initialize();
    void setup();
    color_array* const color_array_ptr();
    color_array* const color_array_all_ptr();
    void render();
    void gather();
    int return_rank() const;
    int return_size() const;
    void barrier() const;

protected:
    bool cam_type;
    int size, rank;
    int width_per_node, height_per_node;
    int width_min, width_max, height_min, height_max;
    camera_derived* cam_derived;
    camera_parallel* cam;
    hittable_list* world;
    hittable_list_parallel* world_parallel;
    MPI_Comm MPI_world;
    color_array c_array, c_array_all;
};

#endif
