#ifndef PLANES_H
#define PLANES_H

#include "quad.h"

class triangle : public quad
{
public:
	triangle(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat);
	bool is_interior(double _a, double _b, hit_record& _rec) const override;

};

class circle : public quad
{
public:
	circle(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat);
	bool is_interior(double _a, double _b, hit_record& _rec) const override;
};


#endif