#ifndef QUAD_GPU_OPENMP_H
#define QUAD_GPU_OPENMP_H

#include "quad.h"
#include "hit_record_gpu_openmp.h"


#pragma omp declare target
class quad_gpu_openmp {
public:
	quad_gpu_openmp() = default;
	quad_gpu_openmp(const quad& _quad, const int& _mat_id);
	void initialize();
	bool hit(const ray& _r, interval _ray_t, hit_record_gpu_openmp& _rec) const;
	bool is_interior(double _a, double _b, hit_record_gpu_openmp& rec) const;


protected:
	point3 Q;
	vec3 u, v;
	vec3 w;
	int material_id;
	vec3 normal;
	double D;
};

#pragma omp end declare target
#endif
