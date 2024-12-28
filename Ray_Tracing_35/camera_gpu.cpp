#include "camera_gpu.h"

camera_gpu::camera_gpu(int _num_streams) : num_streams(_num_streams)
{
	streams = new hipStream[num_streams];

	for (int i = 0; i < num_streams; i++)
		hipStreamCreate(streams[i]);
}

camera_gpu::~camera_gpu()
{
	for (int i = 0; i < num_streams; i++)
		hipStreamDestroy(streams[i]);

	delete[] streams;
}


void camera_gpu::render()
{
	dim3 blockDim(256);
	dim3 gridDim(64)

	for (int i = 0; i < num_streams; i++)
	{
		double shared_memory_size = 0.0;
		copy<HOST_TO_DEVICE>(i);
		render_stream<<<gridDim,blockDim,shared_memory_size,streams[i]>>>(i);
		copy <DEVICE_TO_HOST>(i);
	}
}


template <int direction>
void camera_gpu::copy(int _stream_number)
{
	hipStream stream = streams[stream_number];
	int hittable_size = sizeof(hittable);
	int color_array_size = c_a_h->return_color_data_size();

	if (direction == DEVICE_TO_HOST) {
		hipMemcpyAsync(world_d, world_h, hittable_size, hipMemcpyDeviceToHost, stream);
		hipMemcpyAsync(c_a_d, c_a_h, color_array_size, hipMemcpyDeviceToHost, stream);
	}
	else if (direction == HOST_TO_DEVICE)
	{
		hipMemcpyAsync(world_d, world_h, hittable_size, hipMemcpyHostToDevice, stream);
		hipMemcpyAsync(c_a_d, c_a_h, color_array_size, hipMemcpyHostToDevice, stream);
	}
}



__global__ void camera_gpu::render_stream(int stream_number)
{

	int imin = min_stream_width[stream_number];
	int imax = max_stream_width[stream_number];
	int jmin = min_stream_height[stream_number];
	int jmax = max_stream_height[stream_number];


	int idx = threadIdx.x + blockDim.x * blockIdx.x;
	int idy = threadIdx.y + blockDim.y * blockIdx.y;

	int totalThreadsx = blockDim.x * gridDim.x;
	int totalThreadsy = blockDim.y * gridDim.y;

	int data_per_thread_x = (imax - imin) / totalThreadsx;
	int data_per_thread_y = (jmax - jmin) / totalThreadsy;

	int imin = min_stream_width[stream_number] * data_per_thread_x;
	int jmin = min_stream_height[stream_number] * data_per_thread_y;
	int imax = imin + data_per_thread_x < max_stream_width[stream_number] ? imin + data_per_thread_x : max_stream_width[stream_number];
	int jmax = jmin + data_per_thread_y < max_stream_height[stream_number] ? jmin + data_per_thread_y : max_stream_height[stream_number];


	for (int j = jmin; j < jmax; j++)
	{
		for (int i = imin; i < imax; i++)
		{
			color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world);
			}
			pixel_color = pixel_samples_scale * pixel_color;
		}
	}
}
