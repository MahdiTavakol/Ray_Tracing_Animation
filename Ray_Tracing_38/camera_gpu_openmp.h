#ifndef CAMERA_GPU_OPENMP
#define CAMERA_GPU_OPENMP

#include <omp.h>

#include "camera.h"
#include "input.h"
#include "hittable_list_gpu_openmp.h"

class camera_gpu_openmp : public camera
{
public:
	camera_gpu_openmp(const input* _in);
	camera_gpu_openmp();


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
