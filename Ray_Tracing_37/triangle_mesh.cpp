#include "triangle_mesh.h"

triangle_mesh::triangle_mesh(const std::vector<point3>& _vs, const std::vector<point3>& _vts,
	const std::vector<point3>& _vns, shared_ptr<material> _mat)
	: vs(_vs), vts(_vts), vns(_vns), mat(_mat)
{
	initialize();
}

void triangle_mesh::initialize()
{
	auto n1 = cross(vs[1] - vs[0], vs[2] - vs[0]);
	unit_n1 = unit_vector(n1);
	Q1 = vs[0];
	D1 = dot(unit_n1, Q1);
	w1 = n1 / dot(n1, n1);

	set_bounding_box();
}

void triangle_mesh::set_bounding_box()
{
	auto bbox_diagonal1 = aabb(vs[0], vs[1]);
	auto bbox_diagonal2 = aabb(vs[0], vs[2]);
	auto bbox_diagonal3 = aabb(vs[1], vs[2]);

	auto bbox_diagonal = aabb(bbox_diagonal1, bbox_diagonal2);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal3);
	bbox = bbox_diagonal;
}

bool triangle_mesh::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
	auto denom1 = dot(unit_n1, _r.direction());

	if (std::fabs(denom1) < 1e-8)
		return false;

	auto t1 = (D1 - dot(unit_n1, _r.origin())) / denom1;
	if (!_ray_t.contains(t1))
		return false;


	auto intersection = _r.at(t1);
	vec3 planar_hitpt_vector = intersection - Q1;
	auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2] - vs[0]));
	auto beta = dot(w1, cross(vs[1] - vs[0], planar_hitpt_vector));

	if (!is_interior(alpha, beta, _rec))
		return false;


	std::vector<vec3> triangle, normals, textures;
	triangle.push_back(vs[0]);
	triangle.push_back(vs[1]);
	triangle.push_back(vs[2]);
	normals.push_back(vns[0]);
	normals.push_back(vns[1]);
	normals.push_back(vns[2]);
	textures.push_back(vts[0]);
	textures.push_back(vts[1]);
	textures.push_back(vts[2]);
	auto normal = interpolate(triangle, normals, intersection);
	auto texture = interpolate(triangle, textures, intersection);

	_rec.t = t1;
	_rec.p = intersection;
	_rec.u = texture[0];
	_rec.v = texture[1];
	_rec.w = texture[2];
	_rec.mat = mat;
	_rec.set_face_normal(_r, normal);


	return true;
}

bool triangle_mesh::is_interior(double _a, double _b, hit_record& _rec) const {

	if (_a > 0 && _b > 0 && _a + _b < 1)
		return false;

	return true;
}