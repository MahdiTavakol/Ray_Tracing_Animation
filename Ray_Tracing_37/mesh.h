#ifndef MESH_H
#define MESH_H

#include "hittable.h"
#include "hittable_list.h"
#include "quad.h"

class mesh : public hittable {
public:
	mesh(const std::vector<point3>& vs, const std::vector<point3>& vts, const std::vector<point3>& vns, shared_ptr<material> _mat);
	mesh() {}
	virtual void initialize();
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const;

protected:
	std::vector<point3> vs;
	std::vector<point3> vts;
	std::vector<point3> vns;
	shared_ptr<material> mat;

	int num_edges;
	
	aabb bbox;
	vec3 unit_n1, unit_n2;
	vec3 w1, w2;
	point3 Q1, Q2;
	double D1, D2;

	static vec3 interpolate(const std::vector<point3>& _triangle, const std::vector<point3>& _normals, const vec3& _point);
};


#endif