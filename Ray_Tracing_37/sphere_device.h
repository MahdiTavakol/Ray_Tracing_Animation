#pragma once
#ifndef SPHERE_DEVICE_H
#define SPHERE_DEVICE_H

#include "rtweekend.h"

#include "vec3_device.h"
#include "ray_device.h"
#include "hit_record_device.h"
#include "interval_device.h"
//#include "aabb.h"
#include <memory>
#include <algorithm>

class sphere_device
{
public:
    // Stationary Sphere
    __device__ sphere_device(const point3_device& _center, double _radius);

    // Moving Sphere
    __device__ sphere_device(const point3_device& _center1, const point3_device& _center2, double _radius);

    // With Ray as an input
    __device__ sphere_device(const ray_device& _center, double _radius);

    // The copy constructor
    __device__ sphere_device(const sphere_device& _sphere);

    __device__ bool hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const;

    // No bounding_box() for now!

private:
    ray_device center;
    double radius;
    //aabb bbox;

    __device__ static void get_sphere_uv(const point3_device& _p, double& _u, double& _v);

};


#endif
