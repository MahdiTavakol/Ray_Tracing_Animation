#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "mesh.h"

class triangle_mesh : public mesh {
public:
	triangle_mesh(const std::vector<point3>& vs, const std::vector<point3>& vts, const std::vector<point3>& vns, shared_ptr<material> _mat);
	virtual void initialize() override;
	virtual void set_bounding_box() override;
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const override;

protected:
	std::vector<point3> vs;
	std::vector<point3> vts;
	std::vector<point3> vns;
	shared_ptr<material> mat;

	int num_edges;

	aabb bbox;
	vec3 unit_n1;
	vec3 w1;
	point3 Q1;
	double D1;

};


#endif