#ifndef CAMERA_GPU_OPENMP
#define CAMERA_GPU_OPENMP

#include "camera.h"

class camera_gpu_openmp : public camera
{
public:
	camera_gpu_openmp(const input* _in)
	{
		_in->setup_camera(this);
	}

	void render(const hittable& world, color_array& c_a);

	// Execution parameters
	int NX, NY; // Number of threads in each block
	int TX, TY; // Number of blocks in the Grid

protected:

#pragma omp declare target
	color ray_color(const ray& r, int depth, const hittable& world) const;
#pragma omp end declare target


};
#endif