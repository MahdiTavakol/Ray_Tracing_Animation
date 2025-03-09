#ifndef SPHERE_GPU_OPENMP_H
#define SPHERE_GPU_OPENMP_H

#include "hittable.h"
#include "sphere.h"

// It is derived from the sphere simplified for the gpu

class sphere_gpu_openmp
{
public:
    // The default constructor is required for openmp
    sphere_gpu_openmp() = default;     
#pragma omp declare target 
    sphere_gpu_openmp(const sphere& _shape, const int& _mat_id);

    bool hit(const ray& r, interval ray_t, hit_record_gpu_openmp& rec) const;
#pragma omp end declare target



private:
    ray center;
    double radius;
    
    int material_id;

    static void get_sphere_uv(const point3& p, double& u, double& v);
};



#endif
