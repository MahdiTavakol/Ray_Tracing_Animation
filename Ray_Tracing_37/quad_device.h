#ifndef QUAD_DEVICE_H
#define QUAD_DEVICE_H

#include "vec3_device.h"
#include "hit_record_device.h"
#include "interval_device.h"
#include "ray_device.h"
#include "global_scope_device.h"

class quad_device
{
public:
	__device__ quad_device(const point3_device& _Q, const vec3_device& _u, const vec3_device& _v);
	__device__ bool hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const;
	__device__ bool is_interior(double _a, double _b, hit_record_device& _rec) const;

protected:
	point3_device Q;
	vec3_device u, v;
	vec3_device w;
	vec3_device normal;
	double D;
};

#endif
