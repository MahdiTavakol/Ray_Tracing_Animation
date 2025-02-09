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
    parallel(hittable_list* _world, camera* _cam);
    void parallel_initialize();
    void MPI_initialize();
    virtual void setup();
    virtual color_array* const color_array_ptr();
    color_array* const color_array_all_ptr();
    virtual void render();
    virtual void gather();
    int return_rank() const;
    int return_size() const;
    void barrier() const;

protected:
    camera* cam;
    hittable_list* world;


    int size, rank;
    MPI_Comm MPI_world;
    color_array c_array, c_array_all;
};

#endif
