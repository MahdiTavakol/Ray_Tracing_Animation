#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include <memory>

class material;

class hit_record
{
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    double u;
    double v;
    double w;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal);
};

class hittable
{
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box() const = 0;
};

class translate : public hittable
{
public:
    translate(shared_ptr<hittable> _object, const vec3& _offset);

    bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

class rotate_y : public hittable
{
public:
    rotate_y(shared_ptr<hittable> _object, double _angle);

    bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};

#endif
