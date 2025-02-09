#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"


class sphere_list
{
public:
    sphere_list(const point3* _centers, double* _radius, int)
    // Stationary Sphere
    sphere(const point3& _center, double _radius, std::shared_ptr<material> _mat);

    // Moving Sphere
    sphere(const point3& _center1, const point3& _center2, double _radius, std::shared_ptr<material> _mat);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    aabb bounding_box() const override { return bbox; }

    void return_params(ray& _center, double& _radius, std::shared_ptr<material> _mat) const;

    void return_material(std::shared_ptr<material>& _mat) override;


private:
    ray center;
    double radius;
    std::shared_ptr<material> mat;
    aabb bbox;

    static void get_sphere_uv(const point3& p, double& u, double& v);

};



#endif