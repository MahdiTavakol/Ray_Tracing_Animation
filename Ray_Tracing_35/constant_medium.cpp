#include "constant_medium.h"

constant_medium::constant_medium(shared_ptr<hittable> _boundary, double _density, shared_ptr<texture> _tex)
	:boundary(_boundary), neg_inv_density(-1 / _density),
	phase_function(make_shared<isotropic>(_tex))
{}

constant_medium::constant_medium(shared_ptr<hittable> _boundary, double _density, const color& _albedo)
	:boundary(_boundary), neg_inv_density(-1 / _density),
	phase_function(make_shared<isotropic>(_albedo))
{}

bool constant_medium::hit(const ray& _r, interval _ray_t, hit_record& _rec) const  {
	hit_record rec1, rec2;

	if (!boundary->hit(_r, interval::universe, rec1))
		return false;

	if (!boundary->hit(_r, interval(rec1.t + 0.0001, infinity), rec2))
		return false;

	if (rec1.t < _ray_t.min) rec1.t = _ray_t.min;
	if (rec2.t < _ray_t.max) rec2.t = _ray_t.max;

	if (rec1.t >= rec2.t)
		return false;

	if (rec1.t < 0)
		rec1.t = 0;

	auto ray_length = _r.direction().length();
	auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
	auto hit_distance = neg_inv_density * std::log(random_double());

	if (hit_distance > distance_inside_boundary)
		return false;

	_rec.t = rec1.t + hit_distance / ray_length;
	_rec.p = _r.at(_rec.t);

	_rec.normal = vec3(1, 0, 0);
	_rec.front_face = true;
	_rec.mat = phase_function;

	return true;
}