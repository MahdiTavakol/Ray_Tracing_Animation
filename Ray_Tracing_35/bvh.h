#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node : public hittable
{
public:
	bvh_node(hittable_list list);

	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb bbox;

	static bool box_compare(
		const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
	);

	static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return box_compare(a, b, 2);
	}
};


#endif
