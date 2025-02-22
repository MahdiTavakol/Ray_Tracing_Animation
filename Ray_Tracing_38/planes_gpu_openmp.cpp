#include "planes_gpu_openmp.h"

triangle_gpu_openmp::triangle_gpu_openmp(const triangle& _tri, const int& _mat_id)
{
	material_id = _mat_id;
	std::shared_ptr<material> null_mat;
	_tri.return_params(Q, u, v, null_mat);
	initialize();
}

bool triangle_gpu_openmp::is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const
{
	return _a > 0 && _b > 0 && _a + _b < 1;
}

circle_gpu_openmp::circle_gpu_openmp(const circle& _crcl, const int& _mat_id)
{
	material_id = _mat_id;
	std::shared_ptr<material> null_mat;
	_crcl.return_params(Q, u, v, null_mat);
	initialize();
}

bool circle_gpu_openmp::is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const
{
	return std::sqrt((_a - 0.5) * (_a - 0.5) + (_b - 0.5) * (_b - 0.5)) < 0.5;
}