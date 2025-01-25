#include "parallel_camera.h"

parallel_camera::parallel_camera(hittable_list* _world, camera* _cam)
{
    camera_parallel* cam_type_test = dynamic_cast<camera_parallel*>(_cam);
    hittable_list_parallel* world_type_test = dynamic_cast<hittable_list_parallel*>(_world);

    if (!cam_type_test) std::cerr << "Wrong camera type" << std::endl;
    if (world_type_test) std::cerr << "Wrong world type" << std::endl;
}

void parallel_camera::setup()
{
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

color_array* const parallel_camera::color_array_ptr()
{
    return &c_array;
}

void parallel_camera::render()
{
    cam->render(*world, c_array);
}

void parallel_camera::gather()
{
    color_data* colors = c_array.return_array()[0];
    color_data* colors_all = (color_data*)malloc(width_per_node * height_per_node * size * sizeof(color_data));

    int num_double_data = width_per_node * height_per_node * sizeof(color_data) / sizeof(double);

    MPI_Allgather(colors, num_double_data, MPI_DOUBLE, colors_all, num_double_data, MPI_DOUBLE, MPI_world);
    c_array_all = color_array(cam->image_width, cam->image_height, colors_all);
    free(colors_all);
    colors_all = nullptr;
}