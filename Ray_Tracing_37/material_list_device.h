#ifndef MATERIAL_LIST_DEVICE
#define MATERIAL_LIST_DEVICE

#include "material_device.h"


class material_list_device
{
public:
	__device__ material_list_device()
	{
		n_metals = 0;
		n_dielectrics = 0;
		n_lambertians = 0;
	}

	__device__ material_list_device(int _n_metals, double** _albedos, double* _fuzz) :
		n_metals(_n_metals)
	{
		HIP_CHECK(hipMalloc((void**)&metals, n_metals * sizeof(metal_device)));
		for (int i = 0; i < n_metals; i++)
		{
			color_device albedo(_albedos[i][0], _albedos[i][1], _albedos[i][2]);
			new(&metals[i]) metal_device(albedo, _fuzz[i]);
		}
	}

	__host__ material_list_device(int _n_metals, double** _albedos, double* _fuzz) :
		n_metals(_n_metals)
	{
		HIP_CHECK(hipMalloc((void**)&metals, n_metals * sizeof(metal_device)));
		for (int i = 0; i < n_metals; i++)
		{
			color_device albedo(_albedos[i][0], _albedos[i][1], _albedos[i][2]);
			new(&metals[i]) metal_device(albedo, _fuzz[i]);
		}
	}

	__device__ void add(metal_device* _metals, int& _n_metals)
	{
		metals = _metals;
		n_metals = _n_metals;
	}

	__device__ void add(int _n_metals, double** _albedos, double* _fuzz)
	{
		HIP_CHECK(hipMalloc((void**)&metals, _n_metals * sizeof(metal_device)));
		
		for (int i = 0; i < _n_metals; i++) {
			color_device albedo(_albedos[i][0], _albedos[i][1], _albedos[i][2]);
			new(&metals[i]) metal_device(albedo, _fuzz[i]);
		}

		n_metals = _n_metals;
	}

	__device__ void add(int _n_dielectrics, double* refraction_indexes)
	{
		HIP_CHECK(hipMalloc((void**)&dielectrics, _n_dielectrics * sizeof(dielectric_device)));

		for (int i = 0; i < _n_dielectrics; i++) {
			new(&dielectrics[i]) dielectric_device(refraction_indexes[i]);
		}

		n_dielectrics = _n_dielectrics;
	}

	__device__ void add(int _n_lambertians, shared_ptr<texture_device>* textures)
	{
		HIP_CHECK(hipMalloc((void**)&lambertians, _n_lambertians * sizeof(lambertian_device)));

		for (int i = 0; i < _n_lambertians; i++)
			new(&lambertians[i]) lambertian_device(textures[i]);

		n_lambertians = _n_lambertians;
	}

	__device__ ~material_list_device()
	{
		// Deallocating metals
		for (int i = 0; i < n_metals; i++)
			metals[i].~metal_device();

		HIP_CHECK(hipFree(metals));
		n_metals = 0;

		// Deallocating dielectrics
		for (int i = 0; i < n_dielectrics; i++)
			dielectrics[i].~dielectric_device();

		HIP_CHECK(hipFree(dielectrics));
		n_dielectrics = 0;


		// Deallocating dielectrics
		for (int i = 0; i < n_lambertians; i++)
			lambertians[i].~lambertian_device();

		HIP_CHECK(hipFree(lambertians));
		n_lambertians = 0;
	}

	__device__ metal_device& operator()(int i, int j) const {
		if (i == 0)
			return metals[j];
	}

private:
	metal_device* metals;
	int n_metals;

	dielectric_device* dielectrics;
	int n_dielectrics;

	lambertian_device* lambertians;
	int n_lambertians;


};

#endif