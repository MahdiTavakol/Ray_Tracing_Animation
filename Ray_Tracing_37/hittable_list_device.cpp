#include "hittable_list_device.h"

__host__ __device__ hittable_list_device::hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres)
{
	hipMalloc((void**)&spheres, n_spheres * sizeof(sphere_device));
	
	for (int i = 0; i < n_spheres; i++)
		spheres[i] = sphere_device(_centers[i], _radiuses[i]);
}


__host__ __device__ hittable_list_device::hittable_list_device(ray_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres)
{
	hipMalloc((void**)&spheres, n_spheres * sizeof(sphere_device));

	for (int i = 0; i < n_spheres; i++)
		spheres[i] = sphere_device(_centers[i], _radiuses[i]);
}

__host__ __device__ hittable_list_device::hittable_list_device(sphere_device* _spheres, int* _mat_types, int _n_spheres) :
	spheres(_spheres), mat_types(_mat_types), n_spheres(_n_spheres) {}

__host__ __device__ hittable_list_device::~hittable_list_device()
{
	hipFree(spheres);
	n_spheres = 0;
}

__device__ bool hittable_list_device::hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const
{
	hit_record_device temp_rec;
	bool hit_anything = false;
	auto closet_so_far = ray_t.max;

	for (int i = 0; i < n_spheres; i++)
	{
		if (spheres[i].hit())
	}
}

__device__ void hittable_list_device::add()
{

}

__device__ bool hittable_list_device::hit_spheres(const ray_device& r, interval_device ray_t, hit_record_device& rec) const
{
	for (int i = 0; i < n_spheres; i++)
		spheres[i].hit(ray, rec, _hit);




	hit_record_device temp_rec;
	bool hit_anything = false;
	auto closest_so_far = ray.max;

	for (const auto& object : objects)
	{
		if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}
