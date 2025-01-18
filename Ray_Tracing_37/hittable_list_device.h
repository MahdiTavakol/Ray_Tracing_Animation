#ifndef HITTABLE_LIST_DEVICE_H
#define HITTABLE_LIST_DEVICE_H


#include "vec3_device.h"
#include "sphere_device.h"
#include "hit_record_device.h"
#include "interval_device.h"

class hittable_list_device
{
public:
	__host__ __device__ hittable_list_device() {}
	__host__ __device__ hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types, int _n_spheres);
	__host__ __device__ hittable_list_device(ray_device* _centers, double* _radiuses, int* _mat_types, int _n_spheres);
	__host__ __device__ hittable_list_device(sphere_device* _spheres, int* _mat_types, int _n_spheres);
	__host__ __device__ ~hittable_list_device();

	// To avoid branching I prefered to make the hit output void and material == -1 means the background
	__device__ bool hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const;

	__host__ __device__ void add();

private:

	sphere_device* spheres;
	int* mat_types;
	int n_spheres;

	__device__ bool hit_spheres(const ray_device& r, interval_device ray_t, hit_record_device& rec) const;


	// private constructor for the host so that an object of this class will not be created on the host
	hittable_list_device() {}

};

#endif