#include "quad_device.h"

__device__ quad_device::quad_device(const point3_device& _Q, const vec3_device& _u, const vec3_device& _v)
{
	vec3_device n = cross(_u, _v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);
}

__device__ bool quad_device::hit(const ray_device& _r, interval_device _ray_t, hit_record_device& _rec) const
{
	auto denom = dot(normal, _r.direction());

	if (std::fabs(denom) < 1e-8)
		return false;

	auto t = (D - dot(normal, _r.origin())) / denom;
	if (!_ray_t.contains(t))
		return false;

	auto intersection = _r.at(t);
	vec3_device planar_hitpt_vector = intersection - Q;
	auto alpha = dot(w, cross(planar_hitpt_vector, v));
	auto beta = dot(w, cross(u, planar_hitpt_vector));

	if (!is_interior(alpha, beta, _rec))
		return false;

	_rec.t = t;
	_rec.p = intersection;
	_rec.set_face_normal(_r, normal);

	// The hit method on the hittable_list_device later on set the proper values for these two variables.
	_rec.mat_id = -1;
	_rec.mat_type = -1;

	return true;
}

__device__ bool quad_device::is_interior(double _a, double _b, hit_record_device& _rec) const {
	interval_device unit_interval = interval_device(0, 1);

	if (!unit_interval.contains(_a) || !unit_interval.contains(_b))
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;
}