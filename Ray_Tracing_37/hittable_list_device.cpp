#include "hittable_list_device.h"

__device__ hittable_list_device::hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types, int _n_spheres) :
	mat_types(_mat_types), n_spheres(_n_spheres)
{
	hipMalloc((void**) & spheres, n_spheres * sizeof(sphere_device));
	
	for (int i = 0; i < n_spheres; i++)
		spheres[i] = sphere_device(_centers[i], _radiuses[i]);
}

__device__ hittable_list_device::~hittable_list_device()
{
	hipFree(spheres);
	n_spheres = 0;
}

__device__ void hittable_list_device::hit(ray_device& ray, hit_record_device& rec, bool& _hit) const
{
	hit_spheres(ray,rec,_hit);
}

__device__ bool hittable_list_device::hit_spheres(ray_device& ray, hit_record_device& rec, bool& _hit) const
{
	for (int i = 0; i < n_spheres; i++)
		spheres[i].hit(ray, rec, _hit);
}