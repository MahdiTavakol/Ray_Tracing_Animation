#ifndef CAMERA_GPU_H
#define CAMERA_GPU_H

#include "camera.h"
#include "hip\hip_runtime.h"

#define __HIP_PLATFORM_AMD__

enum { DEVICE_TO_HOST, HOST_TO_DEVICE };

class camera_gpu : public camera
{
public:
	camera_gpu(const int _num_streams);
	~camera_gpu();
	void render(const hittable& world, color_array& c_a);

protected:
	template <int direction>
	void copy(int _stream_number);
	__global__ void camera_gpu::render_stream(int stream_number);


	// The color array only in the host since deep copy is not supported
	color_array c_array_h;

	// host pointers
	hittable* world_h;
	color_data*** c_data_matrix_h;

	// memory pointers
	hittable* world_d;
	color_data*** c_data_matrix_d;

	// streams
	int num_streams;
	hipStream* streams;
	int *min_width, *max_width, *min_height, *max_height;


};

#endif