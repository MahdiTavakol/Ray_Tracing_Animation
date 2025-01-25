#include "hittable_list_device.h"

__host__ __device__ hittable_list_device::hittable_list_device():
	n_spheres(0), n_quads(0)
{}

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
	add_spheres( _centers, _radiuses, _mat_types_spheres, _mat_ids_spheres, _n_spheres);
}

__host__ __device__ hittable_list_device::hittable_list_device(sphere_device* _spheres, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres) :
	spheres(_spheres), mat_types_spheres(_mat_types_spheres), mat_ids_spheres(_mat_ids_spheres), n_spheres(_n_spheres) {}

__host__ __device__ hittable_list_device::~hittable_list_device()
{
	// Deleting spheres
	for (int i = 0; i < n_spheres; i++)
		spheres[i].~sphere_device();

	HIP_CHECK(hipFree(spheres));
	HIP_CHECK(hipFree(mat_types_spheres));
	HIP_CHECK(hipFree(mat_ids_spheres));

	n_spheres = 0;

	// Deleting quads
	for (int i = 0; i < n_quads; i++)
		quads[i].~quad_device();

	HIP_CHECK(hipFree(quads));
	HIP_CHECK(hipFree(mat_types_quads));
	HIP_CHECK(hipFree(mat_ids_quads));

	n_quads = 0;
}

__device__ void hittable_list_device::add_spheres(sphere_device* _spheres, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres)
{
	spheres = _spheres;
	mat_types_spheres = _mat_types_spheres;
	mat_ids_spheres = _mat_ids_spheres;
	n_spheres = _n_spheres;
}

__device__ void hittable_list_device::add_spheres(ray_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres)
{
	HIP_CHECK(hipMalloc((void**)&spheres, n_spheres * sizeof(sphere_device)));
	HIP_CHECK(hipMalloc((void**)&mat_types_spheres, n_spheres * sizeof(int)));
	HIP_CHECK(hipMalloc((void**)&mat_ids_spheres, n_spheres * sizeof(int)));

	for (int i = 0; i < n_spheres; i++)
		new(&spheres[i]) sphere_device(_centers[i], _radiuses[i]);
}

__device__ void hittable_list_device::add_quads(quad_device* _quads, int* _mat_types_quads, int* _mat_ids_quads, int _n_quads)
{
	quads = _quads;
	mat_types_quads = _mat_types_quads;
	mat_ids_quads = _mat_ids_quads;
	n_quads = _n_quads;
}

__device__ void hittable_list_device::add_quads(point3_device* _Qs, vec3_device* _us, vec3_device* _vs)
{
	HIP_CHECK(hipMalloc((void**)&quads, n_quads * sizeof(quad_device)));
	HIP_CHECK(hipMalloc((void**)&mat_types_quads, n_quads * sizeof(int)));
	HIP_CHECK(hipMalloc((void**)&mat_ids_quads, n_quads * sizeof(int)));

	for (int i = 0; i < n_quads; i++)
		new(&quads[i]) quad_device(_Qs[i], _us[i], _vs[i]);
}


__device__ bool hittable_list_device::hit(const ray_device& _r, interval_device _ray_t, hit_record_device& _rec) const
{
	interval_device ray_t_copy = _ray_t;
	bool hit_anything = false;

	hit_anything = hit_spheres(_r, ray_t_copy, _rec);
	hit_anything = hit_quads(_r, ray_t_copy, _rec);


	return hit_anything;
}

__device__ bool hittable_list_device::hit_spheres(const ray_device& _r, interval_device& _ray_t_copy, hit_record_device& _rec) const
{
	hit_record_device temp_rec;
	for (int i = 0; i < n_spheres; i++)
	{
		if (spheres[i].hit(_r, _ray_t_copy, temp_rec))
		{
			_ray_t_copy.max = temp_rec.t;
			_rec = temp_rec;
			_rec.mat_type = mat_types_spheres[i];
			_rec.mat_id = mat_ids_spheres[i];
		}
	}
}

__device__ bool hittable_list_device::hit_quads(const ray_device& _r, interval_device& _ray_t_copy, hit_record_device& _rec) const
{
	hit_record_device temp_rec;
	for (int i = 0; i < n_quads; i++)
	{
		if (spheres[i].hit(_r, _ray_t_copy, temp_rec))
		{
			_ray_t_copy.max = temp_rec.t;
			_rec = temp_rec;
			_rec.mat_type = mat_types_quads[i];
			_rec.mat_id = mat_ids_quads[i];
		}
	}

}


