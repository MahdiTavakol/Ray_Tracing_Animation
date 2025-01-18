#ifndef MATERIAL_LIST_DEVICE
#define MATERIAL_LIST_DEVICE

#include "material_device.h"


class material_list_device
{
public:
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

	__device__ ~material_list_device()
	{
		for (int i = 0; i < n_metals; i++)
			metals[i].~metal_device();

		HIP_CHECK(hipFree(metals));
		n_metals = 0;
	}

	__device__ metal_device& operator()(int i, int j) const {
		if (i == 0)
			return metals[j];
	}

private:
	metal_device* metals;
	int n_metals;
};

#endif