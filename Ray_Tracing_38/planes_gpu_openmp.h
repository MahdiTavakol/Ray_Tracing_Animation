#ifndef PLANES_GPU_OPENMP_H
#define PLANES_GPU_OPENMP_H

#include "quad_gpu_openmp.h"

class triangle_gpu_openmp : public quad_gpu_openmp
{
public:
	triangle_gpu_openmp(const triangle& _tri, const int& _mat_id);
	bool is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const override;

};

class circle_gpu_openmp : public quad_gpu_openmp
{
public:
	circle_gpu_openmp(const circle& _crcl, const int& _mat_id);
	bool is_interior(double _a, double _b, hit_record_gpu_openmp& _rec) const override;
};


#endif