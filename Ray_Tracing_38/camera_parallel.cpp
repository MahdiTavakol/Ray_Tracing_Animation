#include "camera_parallel.h"

camera_parallel::camera_parallel(const input* _in)
{
    _in->setup_camera(this);
}

camera_parallel::camera_parallel(const int _width_min, const int _width_max, const int _height_min, const int _height_max)
    : width_min(_width_min), width_max(_width_max), height_min(_height_min), height_max(_height_max) {}

camera_parallel::camera_parallel()
    : width_min(0), width_max(0), height_min(0), height_max(0) {}


void camera_parallel::render(const hittable& world, color_array& c_a)
{
    initialize();
    for (int j = height_min; j < height_max; j++)
    {
        for (int i = width_min; i < width_max; i++)
        {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++)
            {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            color_data** c_data = c_a.return_array();
            pixel_color = pixel_samples_scale * pixel_color;
            c_data[i - width_min][j - height_min].r = pixel_color.x();
            c_data[i - width_min][j - height_min].g = pixel_color.y();
            c_data[i - width_min][j - height_min].b = pixel_color.z();
        }
    }
}

void camera_parallel::set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max)
{
    this->width_min = _width_min;
    this->width_max = _width_max;
    this->height_min = _height_min;
    this->height_max = _height_max;
}
