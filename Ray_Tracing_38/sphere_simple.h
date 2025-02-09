#ifndef SPHERE_SIMPLE_H
#define SPHERE_SIMPLE_H

#include "hittable.h"
#include "sphere.h"

class sphere_simple: public hittable
{
public:
    // It is derived from the sphere simplified for the gpu
    sphere_simple(const sphere& _shape);


    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    bool hit_simple(const ray& r, interval ray_t, hit_record_simple& rec) const;

    aabb bounding_box() const override { return bbox; }

    void return_params(ray& _center, double& _radius, std::shared_ptr<material> _mat);

    void return_material(std::shared_ptr<material>& _mat) override;


private:
    ray center;
    double radius;
    
    material* mat;
    aabb bbox;

    static void get_sphere_uv(const point3& p, double& u, double& v);
};

#endif