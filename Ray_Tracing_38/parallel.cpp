#include "parallel.h"

parallel::parallel(hittable_list* _world, camera* _cam):
    world(_world), cam(_cam)
{
    parallel_initialize();
}


void parallel::parallel_initialize()
{
    MPI_initialize();
    setup();
}

void parallel::MPI_initialize()
{
    // MPI variables
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
}



color_array* const parallel::color_array_all_ptr() {
    return &c_array_all;
}


int parallel::return_rank() const {
    return rank;
}

int parallel::return_size() const {
    return size;
}

void parallel::barrier() const {
    MPI_Barrier(MPI_world);
}
