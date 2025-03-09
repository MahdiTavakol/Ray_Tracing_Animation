#include <cmath>
#include "planes_gpu_openmp.h"

triangle_gpu_openmp::triangle_gpu_openmp(const triangle& _tri, const int& _mat_id):
	material_id(_mat_id)
{
	material_id = _mat_id;
	std::shared_ptr<material> null_mat;
	_tri.return_params(Q, u, v, null_mat);
	initialize();
}

void triangle_gpu_openmp::initialize()
{
	auto n = cross(u, v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

bool triangle_gpu_openmp::hit(const ray& _r, interval _ray_t, hit_record_gpu_openmp& _rec) const
{
	auto denom = dot(normal, _r.direction());

	if (std::fabs(denom) < 1e-8)
		return false;

	auto t = (D - dot(normal, _r.origin())) / denom;
	if (!_ray_t.contains(t))
		return false;

	auto intersection = _r.at(t);
	vec3 planar_hitpt_vector = intersection - Q;
	auto alpha = dot(w, cross(planar_hitpt_vector, v));
	auto beta = dot(w, cross(u, planar_hitpt_vector));

	if (!is_interior(alpha, beta, _rec))
		return false;

	_rec.t = t;
	_rec.p = intersection;
	_rec.set_face_normal(_r, normal);

	return true;
}

bool triangle_gpu_openmp::is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const {

	if (_a <= 0.0 || _b <= 0.0 || _a + _b >= 1.0)
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;

}


circle_gpu_openmp::circle_gpu_openmp(const circle& _crl, const int& _mat_id):
	material_id(_mat_id)
{
	material_id = _mat_id;
	std::shared_ptr<material> null_mat;
	_tri.return_params(Q, u, v, null_mat);
	initialize();
}

void circle_gpu_openmp::initialize()
{
	auto n = cross(u, v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

bool circle_gpu_openmp::hit(const ray& _r, interval _ray_t, hit_record_gpu_openmp& _rec) const
{
	auto denom = dot(normal, _r.direction());

	if (std::fabs(denom) < 1e-8)
		return false;

	auto t = (D - dot(normal, _r.origin())) / denom;
	if (!_ray_t.contains(t))
		return false;

	auto intersection = _r.at(t);
	vec3 planar_hitpt_vector = intersection - Q;
	auto alpha = dot(w, cross(planar_hitpt_vector, v));
	auto beta = dot(w, cross(u, planar_hitpt_vector));

	if (!is_interior(alpha, beta, _rec))
		return false;

	_rec.t = t;
	_rec.p = intersection;
	_rec.set_face_normal(_r, normal);

	return true;
}

bool circle_gpu_openmp::is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const {

	if (sqrt((_a - 0.5) * (_a - 0.5) + (_b - 0.5) * (_b - 0.5)) >= 0.5)
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;

}
