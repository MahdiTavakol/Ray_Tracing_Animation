#include "quad_gpu_openmp.h"

quad_gpu_openmp::quad_gpu_openmp(const quad& _quad, const int& _mat_id):
	material_id(_mat_id)
{
	std::shared_ptr<material> null_mat;
	_quad.return_params(Q, u, v, null_mat);

	initialize();
}

void quad_gpu_openmp::initialize()
{
	auto n = cross(u, v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

void quad_gpu_openmp::set_bounding_box()
{
	auto bbox_diagonal1 = aabb(Q, Q + u + v);
	auto bbox_diagonal2 = aabb(Q + u, Q + v);
	bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

bool quad_gpu_openmp::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
	std::cerr << "For CPU usage please use the quad class instead " << std::endl;
}

bool quad_gpu_openmp::hit(const ray& _r, interval _ray_t, hit_record_gpu_openmp& _rec) const
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

bool quad_gpu_openmp::is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const {
	interval unit_interval = interval(0, 1);

	if (!unit_interval.contains(_a) || !unit_interval.contains(_b))
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;

}