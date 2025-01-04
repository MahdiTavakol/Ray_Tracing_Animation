#include "camera_gpu.h"

camera_gpu::~camera_gpu()
{
	for (int i = 0; i < num_streams; i++)
		hipStreamDestroy(streams[i]);

	hipHostFree(c_data_h);
	hipFree(c_data_d);
}



void camera_gpu::render(color_array& c_a)
{
	for (int i = 0; i < num_streams; i++)
	{
		render_stream(i);
		copy_device_to_host(i);
	}

	// Synchronize stream
	for (int i = 0; i < num_streams; i++)
		hipStreamSynchronize(streams[i]);

	gather_streams(c_a);
}

void camera_gpu::copy_device_to_host(const int _stream_number)
{
	hipStream stream = streams[_stream_number];
	int hittable_size = sizeof(hittable);
	
	int data_size = data_num_stream[_stream_number] * sizeof(color_data);

	int offset = min_stream_width[_stream_number] + image_width * min_stream_height[_stream_number];

	/*
	*  We setup the world on the device and then asks it for the color_data_matrix
	*  So for the DEVICE_TO_HOST we just need to transfer the color_data_matrix
	*  and in the opposite direction there is a need for just world_d
	*/
	
	HIP_CHECK(hipMemcpyAsync(c_data_h+offset, c_data_d+offset, data_size, hipMemcpyDeviceToHost, stream));
}

void camera_gpu::gather_streams(color_array& c_a)
{
	color_data** c_data = c_a.return_array();

	for (int i = 0; i < image_width; i++)
		for (int j = 0; j < image_height; j++)
		{
			int index = i + j * image_width;
			c_data[i][j] = c_data_h[index];
		}
}

void camera_gpu::initialize_memory()
{
	hipMallocHost((void **) & c_data_h, image_width * image_height * sizeof(color_data));
	hipMalloc((void **) & c_data_d, image_width * image_height * sizeof(color_data));
}

void camera_gpu::initialize_streams()
{
	for (int i = 0; i < num_streams; i++)
		hipStreamCreate(&streams[i]);
}


