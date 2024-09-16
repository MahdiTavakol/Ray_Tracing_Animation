#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include <memory>
#include <algorithm>

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(const point3& _center, double _radius, std::shared_ptr<material> _mat);

    // Moving Sphere
    sphere(const point3& _center1, const point3& _center2, double _radius, std::shared_ptr<material> _mat);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

private:
    ray center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif
