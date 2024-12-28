#include "mesh.h"

mesh::mesh(const std::vector<point3>& _vs, const std::vector<point3>& _vts, 
	const std::vector<point3>& _vns, shared_ptr<material> _mat)
	: vs(_vs), vts(_vts), vns(_vns), mat(_mat), num_edges(_vs.size())
{
	initialize();
}

void mesh::initialize()
{
	//std::cout << FLERR;
	//std::cout << "Started Initializing " << std::endl;
	auto n1 = cross(vs[1] - vs[0], vs[2] - vs[0]);
	auto n2 = cross(vs[1] - vs[3], vs[2] - vs[3]);
	unit_n1 = unit_vector(n1);
	unit_n2 = unit_vector(n2);
	Q1 = vs[0];
	Q2 = vs[3];
	D1 = dot(unit_n1, Q1);
	D2 = dot(unit_n2, Q2);
	w1 = n1 / dot(n1, n1);
	w2 = n2 / dot(n2, n2);

	set_bounding_box();

	//std::cout << FLERR;
	//std::cout << "Finished Initializing " << std::endl;
}

void mesh::set_bounding_box()
{
	auto bbox_diagonal1 = aabb(vs[0], vs[1]);
	auto bbox_diagonal2 = aabb(vs[0], vs[2]);
	auto bbox_diagonal3 = aabb(vs[0], vs[3]);
	auto bbox_diagonal4 = aabb(vs[1], vs[2]);
	auto bbox_diagonal5 = aabb(vs[1], vs[3]);
	auto bbox_diagonal6 = aabb(vs[2], vs[3]);

	auto bbox_diagonal = aabb(bbox_diagonal1, bbox_diagonal2);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal3);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal4);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal5);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal6);
	bbox = bbox_diagonal;
}

bool mesh::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
	auto denom1 = dot(unit_n1, _r.direction());
	auto denom2 = dot(unit_n2, _r.direction());

	if (std::fabs(denom1) < 1e-8 || std::fabs(denom2) < 1e-8)
		return false;

	auto t1 = (D1 - dot(unit_n1, _r.origin())) / denom1;
	auto t2 = (D2 - dot(unit_n2, _r.origin())) / denom2;
	if (!_ray_t.contains(t1) && !_ray_t.contains(t2))
		return false;

	if (_ray_t.contains(t1))
	{
		auto intersection = _r.at(t1);
		vec3 planar_hitpt_vector = intersection - Q1;
		auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2]-vs[0]));
		auto beta = dot(w1, cross(vs[1]-vs[0], planar_hitpt_vector));

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
		_rec.set_face_normal(_r,normal);
	}
	if (_ray_t.contains(t2))
	{
		auto intersection = _r.at(t2);
		vec3 planar_hitpt_vector = intersection - Q2;
		auto alpha = dot(w1, cross(planar_hitpt_vector, vs[1] - vs[3]));
		auto beta = dot(w1, cross(vs[2] - vs[3], planar_hitpt_vector));

		if (!is_interior(alpha, beta, _rec))
			return false;

		std::vector<vec3> triangle, normals, textures;
		triangle.push_back(vs[1]);
		triangle.push_back(vs[2]);
		triangle.push_back(vs[3]);
		normals.push_back(vns[1]);
		normals.push_back(vns[2]);
		normals.push_back(vns[3]);
		textures.push_back(vts[1]);
		textures.push_back(vts[2]);
		textures.push_back(vts[3]);
		auto normal = interpolate(triangle, normals, intersection);
		auto texture = interpolate(triangle, textures, intersection);

		_rec.t = t1;
		_rec.p = intersection;
		_rec.u = texture[1];
		_rec.v = texture[2];
		_rec.w = texture[3];
		_rec.mat = mat;
		_rec.set_face_normal(_r, normal);
	}


	return true;
}

bool mesh::is_interior(double _a, double _b, hit_record& _rec) const {
	
	if (_a > 0 && _b > 0 && _a + _b < 1)
		return false;

	return true;
}

vec3 mesh::interpolate(const std::vector<point3>& _triangle, const std::vector<point3>& _normals, const vec3& _point)
{
	vec3 PA = _point - _triangle[0];
	vec3 PB = _point - _triangle[1];
	vec3 PC = _point - _triangle[2];
	vec3 AB = _triangle[1] - _triangle[0];
	vec3 AC = _triangle[2] - _triangle[0];

	auto Apbc = 0.5 * dot(PB, PC);
	auto Apac = 0.5 * dot(PA, PC);
	auto Apab = 0.5 * dot(PA, PB);
	auto Aabc = 0.5 * dot(AB, AC);

	return ((Apbc / Aabc) * _normals[0] + (Apac / Aabc) * _normals[1] + (Apab / Aabc) * _normals[2]);
}