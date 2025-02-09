#include "planes.h"

triangle::triangle(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat):
	quad(_Q, _u, _v, _mat) {}

bool triangle::is_interior(double _a, double _b, hit_record& _rec) const
{
	return _a > 0 && _b > 0 && _a + _b < 1;
}

circle::circle(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat) :
	quad(_Q, _u, _v, _mat) {}

bool circle::is_interior(double _a, double _b, hit_record& _rec) const
{
	return std::sqrt((_a-0.5)*(_a-0.5)+(_b-0.5)*(_b-0.5)) < 0.5;
}