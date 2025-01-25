#ifndef GLOBAL_SCOPE_DEVICE_H
#define GLOBAL_SCOPE_DEVICE_H


#include "color_device.h"

// primary parameters set by the class input;
__device__ double aspect_ratio = 1.0;
__device__ int image_width = 100;
__device__ int image_height;
__device__ int samples_per_pixel = 10;
__device__ int max_depth = 10;
__device__ color_device background;
__device__ double vfov = 90;
__device__ point3_device lookfrom = point3_device(0, 0, 0);
__device__ point3_device lookat = point3_device(0, 0, -1);
__device__ vec3_device vup = vec3_device(0, 1, 0);
__device__ double defocus_angle = 0;
__device__ double focus_dist = 10;




// secondary parameters set by the initialize method
__device__ double pixel_samples_scale;
__device__ point3_device center;
__device__ point3_device pixel00_loc;
__device__ vec3_device pixel_delta_u;
__device__ vec3_device pixel_delta_v;
__device__ vec3_device u, v, w;
__device__ vec3_device defocus_disk_u;
__device__ vec3_device defocus_disk_v;



__device__ const double infinity_device = std::numeric_limits<double>::infinity();
__device__ const double pi_device = 3.1415926535897932385;

__device__ inline double degrees_to_radians_device(double degree)
{
	return degree * pi / 180.0;
}

__device__ inline double random_double_device()
{
	return std::rand() / (RAND_MAX + 1.0);
}

__device__ inline double random_double_device(double min, double max)
{
	return min + (max - min) * random_double_device();
}

__device__ inline int random_int_device(int min, int max)
{
	return int(random_double_device(min, max + 1));
}

#endif