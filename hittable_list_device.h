#ifndef HITTABLE_LIST_DEVICE_H
#define HITTABLE_LIST_DEVICE_H


#include "vec3_device.h"
#include "sphere_device.h"
#include "hit_record_device.h"

class hittable_list_device
{
public:
	__device__ hittable_list_device() {}
	__device__ hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types, int _n_spheres);
	__device__ ~hittable_list_device();

	// To avoid branching I prefered to make the hit output void and material == -1 means the background
	__device__ void hit(ray_device& ray, hit_record_device& rec, bool& _hit) const;

	__device__ void add();

private:
	sphere_device* spheres;
	int* mat_types;
	int n_spheres;

	__device__ bool hit_spheres(ray_device& ray, hit_record_device& rec, bool& _hit) const;

};

#endif