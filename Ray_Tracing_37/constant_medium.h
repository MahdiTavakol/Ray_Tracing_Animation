#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
public:
	constant_medium(shared_ptr<hittable> _boundary, double _density, shared_ptr<texture> _tex);

	constant_medium(shared_ptr<hittable> _boundary, double _density, const color& _albedo);

	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

	aabb bounding_box() const override { return boundary->bounding_box(); }

private:
	shared_ptr<hittable> boundary;
	double neg_inv_density;
	shared_ptr<material> phase_function;
};

#endif