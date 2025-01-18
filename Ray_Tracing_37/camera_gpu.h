#ifndef CAMERA_GPU_H
#define CAMERA_GPU_H

#include <unordered_set>

#include "rtweekend.h"
#include "rtweekend_gpu.h"


#include "ray_device.h"
#include "vec3_device.h"
#include "color_device.h"
#include "hittable_list.h"
#include "hittable_list_device.h"
#include "material_list_device.h"
#include "sphere.h"
#include "material.h"
#include "color_array.h"
#include "global_scope_device.h"

enum { DEVICE_TO_HOST, HOST_TO_DEVICE };

using color = vec3_device;

class camera_gpu
{
	friend class input;
	friend class parallel;
public:
	camera_gpu(hittable_list* _world_h);
	~camera_gpu();

	void render(color_array& c_a);


	__device__ virtual void move_camera(point3_device _lookfrom) {
		::lookfrom = _lookfrom;
	}


	__global__ __device__ void print_back_ground() const
	{
		std::cout << background << std::endl;
	}

	


protected:


	// Initialize the memory
	void allocate_memory();

	/*
     * These two functions read the world_h (hittable_list) on the host and fill out
     * the world_d (hittable_list_device) and materials on the device.
     */
	// Copies the world_h contents to the device
	void initialize_device_memory();
	// Fills the world_d based on the world_h contents
	void fill_world_d();
	void fill_material_d();
	// Deallocates the world_d
	__global__ void destroy_world_d();

	__global__ void create_spheres_on_device(int _n_spheres, double** _sphere_centers_d, double* _sphere_radii_d, int* _sphere_mat_type_d, int* _sphere_mat_id_d);
	__global__ void create_metals_on_device(int _n_metals, double** _albedo_d, double* _fuzz_d);
	
	// Initialize streams
	void initialize_streams();

	// Device functions
	__device__ ray_device get_ray(int i, int j) const;
	__device__ vec3_device sample_square() const;
	__device__ point3_device defocus_disk_sample() const;
	__device__ color ray_color(const ray_device& r, int depth, const hittable_list_device* world) const;
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
	hittable_list* world_h;
	hittable_list_device* world_d;

	// Materials.. For now I used the host class on purpose so if it is possible or not
	//material* materials_h;
	//material* materials_d;

	// Material_list on the device
	material_list_device* material_d;

	// Shape parameters on device and host
	int n_spheres;
	double** sphere_centers_h, * sphere_radii_h;
	double** sphere_centers_d, * sphere_radii_d;
	int* sphere_mat_type_d, * sphere_mat_type_h;
	int* sphere_mat_id_d, * sphere_mat_id_h;

	// Material parameters on device and host
	int n_metals;
	double** albedo_h, * fuzz_h;
	double** albedo_d, * fuzz_d;


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

