#ifndef HITTABLE_LIST_DEVICE_H
#define HITTABLE_LIST_DEVICE_H


#include "vec3_device.h"
#include "sphere_device.h"
#include "hit_record_device.h"
#include "interval_device.h"
#include "quad_device.h"

class hittable_list_device
{
public:
	__host__ __device__ hittable_list_device() {}
	__host__ __device__ hittable_list_device(vec3_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres);
	__host__ __device__ hittable_list_device(ray_device* _centers, double* _radiuses, int* _mat_types_spheres, int* mat_ids_spheres, int _n_spheres);
	__host__ __device__ hittable_list_device(sphere_device* _spheres, int* _mat_types_spheres, int* mat_ids_spheres, int _n_spheres);
	__host__ __device__ ~hittable_list_device();

	// Adding spheres
	__device__ void add_spheres(sphere_device* _spheres, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres);
	__device__ void add_spheres(ray_device* _centers, double* _radiuses, int* _mat_types_spheres, int* _mat_ids_spheres, int _n_spheres);

	// Adding quads
	__device__ void add_quads(quad_device* _quads, int* _mat_types_quads, int* _mat_ids_quads, int _n_quads);
	__device__ void add_quads(point3_device* _Qs, vec3_device* _us, vec3_device* _vs);

	// To avoid branching I prefered to make the hit output void and material == -1 means the background
	__device__ bool hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const;


private:
	// Structure of Arrays for each type rather than array of structures for a better access pattern???

	// Spheres
	sphere_device* spheres;
	int* mat_types_spheres;
	int* mat_ids_spheres;
	int n_spheres;

	// Quades
	quad_device* quads;
	int* mat_types_quads;
	int* mat_ids_quads;
	int n_quads;

	// Other types



	__device__ bool hit_spheres(const ray_device& r, interval_device& ray_t, hit_record_device& rec) const;
	__device__ bool hit_quads(const ray_device& r, interval_device& ray_t, hit_record_device& rec) const;


	// Other types

	// private constructor for the host so that an object of this class will not be created on the host
	hittable_list_device() {}

};

#endif