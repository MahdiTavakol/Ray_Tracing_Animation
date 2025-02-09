#include "input.cpp"
#include "global_scope_device.h"


__global__ void input::setup_gpu_camera()
{

	::lookfrom = point3_device(13, 2, 3);
	::lookat = point3_device(0, 0, 0);
	::vup = point3_device(0, 1, 0);


	::defocus_angle = 0.6;
	::defocus_angle = 0;
	::focus_dist = 10.0;
	::background = color_device(0.7, 0.8, 1.00);

}