#include "hittable_list_device.h"

__host__ __device__ hittable_list_device::hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres)
{
	HIP_CHECK(hipMalloc((void**)&spheres, n_spheres * sizeof(sphere_device)));
	HIP_CHECK(hipMalloc((void**)&mat_types_spheres, n_spheres * sizeof(int)));
	HIP_CHECK(hipMalloc((void**)&mat_ids_spheres, n_spheres * sizeof(int)));
	
	for (int i = 0; i < n_spheres; i++)
		spheres[i] = sphere_device(_centers[i], _radiuses[i]);
}


__host__ __device__ hittable_list_device::hittable_list_device(ray_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres)
{
	HIP_CHECK(hipMalloc((void**)&spheres, n_spheres * sizeof(sphere_device)));
	HIP_CHECK(hipMalloc((void**)&mat_types_spheres, n_spheres * sizeof(int)));
	HIP_CHECK(hipMalloc((void**)&mat_ids_spheres, n_spheres * sizeof(int)));

	for (int i = 0; i < n_spheres; i++)
		spheres[i] = sphere_device(_centers[i], _radiuses[i]);
}

__host__ __device__ hittable_list_device::hittable_list_device(sphere_device* _spheres, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	spheres(_spheres), mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres) {}

__host__ __device__ hittable_list_device::~hittable_list_device()
{
	for (int i = 0; i < n_spheres; i++)
		spheres[i].~sphere_device();

	HIP_CHECK(hipFree(spheres));
	HIP_CHECK(hipFree(mat_types_spheres));
	HIP_CHECK(hipFree(mat_ids_spheres));

	n_spheres = 0;
}

__device__ bool hittable_list_device::hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const
{
	hit_record_device temp_rec;
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	for (int i = 0; i < n_spheres; i++)
	{
		if (spheres[i].hit(r, interval_device(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
			rec.mat_type = mat_types_spheres[i];
			rec.mat_id = mat_ids_spheres[i];
		}
	}

	return hit_anything;
}

__device__ void hittable_list_device::add()
{

}
