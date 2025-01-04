#include "parallel.h"


parallel::parallel(hittable_list* _world, camera_parallel* _cam) :
    world(_world), cam(_cam), cam_type(PARALLEL_CAMERA)
{
    MPI_initialize();
    setup();
}

parallel::parallel(hittable_list_parallel* _world_parallel, camera_derived* _cam_derived) :
    world_parallel(_world_parallel), cam_derived(_cam_derived), cam_type(CAMERA)
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

void parallel::setup()
{
    // distribute load
    if (cam_type == PARALLEL_CAMERA) {
        width_per_node = int(cam->image_width / size + 1);
        height_per_node = cam->image_height;
        width_min = rank * width_per_node;
        width_max = width_min + width_per_node;
        height_min = 0;
        height_max = cam->image_height;
        cam->set_range(width_min, width_max, height_min, height_max);

        // colors_array
        c_array.reset_size(width_per_node, height_per_node);
    }
    else if (cam_type == CAMERA)
    {
        c_array_all.reset_size(cam_derived->image_width, cam_derived->image_height);
    }
}

color_array* const parallel::color_array_ptr() {
    if (cam_type == PARALLEL_CAMERA)
        return &c_array;
    else
        std::cerr << "Undefined behavior" << std::endl;
}

color_array* const parallel::color_array_all_ptr() {
    return &c_array_all;
}

void parallel::render() {
    if (cam_type == PARALLEL_CAMERA)
    {
        cam->render(*world, c_array);
    }
    else if (cam_type == CAMERA)
    {
        // In this case the world should be of hittable_list_parallel type
        if (world_parallel == nullptr) std::cout << "World_parallel is nullptr" << std::endl;
        cam_derived->render(*world_parallel, c_array_all);
    }
}

void parallel::gather() {
    if (cam_type == PARALLEL_CAMERA) {
        color_data* colors = c_array.return_array()[0];
        color_data* colors_all = (color_data*)malloc(width_per_node * height_per_node * size * sizeof(color_data));

        int num_double_data = width_per_node * height_per_node * sizeof(color_data) / sizeof(double);

        MPI_Allgather(colors, num_double_data, MPI_DOUBLE, colors_all, num_double_data, MPI_DOUBLE, MPI_world);
        c_array_all = color_array(cam->image_width, cam->image_height, colors_all);
        free(colors_all);
        colors_all = nullptr;
    }
    else
    {
        std::cerr << "The gather method should in the parallel class should be implemented for a non-parallel camera object" << std::endl;
    }
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
