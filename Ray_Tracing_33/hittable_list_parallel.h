#ifndef HITTABLE_LIST_PARALLEL_H
#define HITTABLE_LIST_PARALLEL_H

#include "hittable_list.h"
#include "mpi.h"

struct hit_record_struct
{
	point3 p;
	vec3 normal;
	double t;
	double u;
	double v;
	double w;
	bool front_face;
};

hit_record_struct hit_record_to_struct(const hit_record&);


class hittable_list_parallel : public hittable_list {
public:

	void add(shared_ptr<hittable> object)
	{
		objects.push_back(object);
		//bbox = aabb(bbox, object->bounding_box());
	}


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;


protected:
	aabb bbox;
};

#endif
