#ifndef RAY_DEVICE_H
#define RAY_DEVICE_H

#include "vec3_device.h"

class ray_device
{
public:
	__device__ ray_device() {}

	__device__ ray_device(const point3_device& _orig, const vec3_device& _dir, double _tm) : orig(_orig), dir(_dir), tm(_tm) {}

	__device__ ray_device(const ray_device& _ray) :
		orig(_ray.orig), dir(_ray.dir), tm(_ray.tm) {}

	__device__ ray_device(const point3_device& _orig, const vec3_device& _dir)
		:ray_device(_orig, _dir, 0) {}

	__device__ const point3_device& origin() const { return orig; }
	__device__ const vec3_device& direction() const { return dir; }

	__device__ double time() const { return tm; }

	__device__ point3_device at(double t) const {
		return orig + t * dir;
	}

private:
	point3_device orig;
	vec3_device dir;
	double tm;
};

#endif