#ifndef CAMERA_GPU_H
#define CAMERA_GPU_H

#include "rtweekend.h"
#include "rtweekend_gpu.h"


#include "ray_device.h"
#include "vec3_device.h"
#include "color_device.h"
#include "hittable_list.h"
#include "hittable_list_device.h"
#include "color_array.h"

enum { DEVICE_TO_HOST, HOST_TO_DEVICE };

using color = vec3_device;

class camera_gpu
{
	friend class input;
	friend class parallel;
public:
	camera_gpu()
	{
		initialize_memory();
		initialize_streams();
		initialize_camera<<<1,1>>>();
	}

	void render(color_array& c_a);


	__global__ __device__ virtual void move_camera(point3 _lookfrom) {
		this->lookfrom = _lookfrom;
	}

	__global__ __device__ void print_back_ground() const
	{
		std::cout << background << std::endl;
	}

	~camera_gpu();


protected:

	// primary parameters set by the class input;
	double aspect_ratio = 1.0;
	int image_width = 100;
	int image_height;
	int samples_per_pixel = 10;
	int max_depth = 10;
	color_device background;
	double vfov = 90;
	point3_device lookfrom = point3_device(0, 0, 0);
	point3_device lookat = point3_device(0, 0, -1);
	vec3_device vup = vec3_device(0, 1, 0);
	double defocus_angle = 0;
	double focus_dist = 10;




	// secondary parameters set by the initialize method
	double pixel_samples_scale;
	point3_device center;
	point3_device pixel00_loc;
	vec3_device pixel_delta_u;
	vec3_device pixel_delta_v;
	vec3_device u, v, w;
	vec3_device defocus_disk_u;
	vec3_device defocus_disk_v;

	// Initialize the memory
	void initialize_memory();
	
	// Initialize streams
	void initialize_streams();

	// Device functions
	__device__ ray_device get_ray(int i, int j) const;
	__device__ vec3_device sample_square() const;
	__device__ point3_device defocus_disk_sample() const;
	__device__ color ray_color(const ray_device& r, int depth, const hittable_list_device& world) const;
	__global__ void initialize_camera();



	// GPU specific variables and functions

	// Copying from device to host
	void copy_device_to_host(const int _stream_number);


	// Rendering each stream
	__global__ void render_stream(int stream_number);
	
	// Gathering information from each stream
	void gather_streams(color_array& c_a);

	/* 
	 *  hittables lists on the GPU and on the host side 
	 *  The host hittable_list is a kind of array of object of different types 
	 * (array of structures AoS)
	 * that for each object it contains the hit method and the material type.
	 * Each hit method is invoked through polymorphism and since the dynamic 
	 * polymorphism is not supported in GPU programming, I decided to organized 
	 * the hittable_list in the GPU as structure of arrays. Then, during rendering
	 * each member of array[i] of the SoA just hit_method[i] in called.
	 * For the copy the host reads each object in the world_h and creats the same
	 * structure in the world_d instead of copying everything.
	 */
	hittable_list world_h;
	hittable_list_device world_d;

	/*
	 * This function read the hittable_list on the host and fills out the hittable_list and materials on the device
	 */
	__global__ void hittable_list_H_to_D();

	// The color array only in the host since deep copy is not supported;
	color_array* c_array_h;

	// host pointers 
	color_data* c_data_h;

	// memory pointers
	color_data* c_data_d;

	// streams
	int num_streams;
	hipStream* streams;
	int *min_stream_width, *max_stream_width, *min_stream_height, *max_stream_height;
	int* data_num_stream;


};

#endif

