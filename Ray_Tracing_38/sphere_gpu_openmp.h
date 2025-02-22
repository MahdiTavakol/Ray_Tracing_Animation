#ifndef SPHERE_GPU_OPENMP_H
#define SPHERE_GPU_OPENMP_H

#include "hittable.h"
#include "sphere.h"

class sphere_gpu_openmp: public hittable
{
public:
    // It is derived from the sphere simplified for the gpu
    sphere_gpu_openmp(const sphere& _shape, const int& _mat_id);


    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    bool hit(const ray& r, interval ray_t, hit_record_gpu_openmp& rec) const;

    aabb bounding_box() const override { return bbox; }



private:
    ray center;
    double radius;
    
    int material_id;
    aabb bbox;

    static void get_sphere_uv(const point3& p, double& u, double& v);
};

#endif