#ifndef QUAD_GPU_OPENMP_H
#define QUAD_GPU_OPENMP_H

#include "hittable.h"
#include "hittable_list.h"

class quad_gpu_openmp : public hittable{
public:
	quad_gpu_openmp(){}
	quad_gpu_openmp(const quad& _quad, const int& _mat_id);
	void initialize();
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	bool hit(const ray& _r, interval _ray_t, hit_record_gpu_openmp& _rec) const;
	virtual bool is_interior(double _a, double _b, hit_record_gpu_openmp& rec) const;


protected:
	point3 Q;
	vec3 u, v;
	vec3 w;
	int material_id;
	aabb bbox;
	vec3 normal;
	double D;
};


#endif
