#include "quad.h"

quad::quad(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat)
	: Q(_Q), u(_u), v(_v), mat(_mat)
{
	auto n = cross(_u, _v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

void quad::set_bounding_box()
{
	auto bbox_diagonal1 = aabb(Q, Q + u + v);
	auto bbox_diagonal2 = aabb(Q + u, Q + v);
	bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

bool quad::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
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
	_rec.mat = mat;
	_rec.set_face_normal(_r, normal);

	return true;
}

bool quad::is_interior(double _a, double _b, hit_record& _rec) const {
	interval unit_interval = interval(0, 1);

	if (!unit_interval.contains(_a) || !unit_interval.contains(_b))
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;
}