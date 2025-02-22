#ifndef HITTABLE_LIST_GPU_OPENMP_H
#define HITTABLE_LIST_GPU_OPENMP_H

#include "rtweekend.h"

#include "aabb.h"
#include "hittable.h"
#include "hit_record_gpu_openmp.h"
#include "interval.h"
#include "sphere.h"
#include "quad.h"
#include "planes.h"

#include <vector>



class hittable_list_gpu_openmp : public hittable {
	friend class camera_gpu_openmp;
public:
	hittable_list_gpu_openmp();
	hittable_list_gpu_openmp(shared_ptr<hittable> _object);
	~hittable_list_gpu_openmp();
	void initialize();
	void clear();
	virtual void add(shared_ptr<hittable> object)
	{
		objects.push_back(object);
		bbox = aabb(bbox, object->bounding_box());
	}
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
	bool hit(const ray& r, interval ray_t, int _mat_info[2]) const;
	aabb bounding_box() const override { return bbox; }


protected:

	aabb bbox;

	/* First we put everything into the objects array to know
	 * The number of each of spheres, quads, etc. then we allocate
	 * the arrays and deallocate the objects vector.
	 * The reason for doing this is that GPU neither support dynamic
	 * polymorphism nor support deep copies.
	 */
	std::vector<std::shared_ptr<hittable>> objects;

	// A flag to check if the spheres, ... arrays have been allocated. After that no new items can be added.
	bool allocated;



	// -----> This part should have its own class
	// Shapes
	int nshapes;
	int** shapes_mat_info; // col0 --> mat_type , col1-->mat_id

	int nspheres;
	int* sphere_ids;
	sphere* spheres;
	int nquads;
	int* quad_ids;
	quad* quads;
	int ntriangles;
	int* triangle_ids;
	triangle* triangles;
	int* circle_ids;
	int ncircles;
	circle* circles;
	// <----- This part should have its own class


	// -----> This part should have its own class
	// Materials
	int nmaterials;

	int nmetals;
	metal* metals;
	int nlambertians;
	lambertian* lambertians;
	int ndielectrics;
	dielectric* dielectrics;
	// <----- This part should have its own class

};

#endif