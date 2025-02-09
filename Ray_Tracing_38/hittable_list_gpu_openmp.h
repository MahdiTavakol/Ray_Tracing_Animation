#ifndef HITTABLE_LIST_GPU_OPENMP_H
#define HITTABLE_LIST_GPU_OPENMP_H

#include "rtweekend.h"

#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "sphere.h"
#include "quad.h"

#include <vector>



class hittable_list_gpu_openmp : public hittable {
public:
	hittable_list_gpu_openmp();
	hittable_list_gpu_openmp(shared_ptr<hittable> _object);
	~hittable_list_gpu_openmp();
	void clear();
	virtual void add(shared_ptr<hittable> object);
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;


protected:
	int nlocal; // number of objects
	int nmax; // max objects
	aabb bbox;
	hittable** objects;
};

#endif