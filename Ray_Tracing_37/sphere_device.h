#pragma once
#ifndef SPHERE_DEVICE_H
#define SPHERE_DEVICE_H

#include "rtweekend.h"

#include "vec3_device.h"
#include "ray_device.h"
#include "hit_record_device.h"
#include "interval.h"
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

    // The copy constructor
    __device__ sphere_device(const sphere_device& _sphere);

    __device__ void hit(ray_device& _ray, hit_record_device& _rec, bool& _hit) const;

    // No bounding_box() for now!

private:
    ray_device center;
    double radius;
    //aabb bbox;

    __device__ static void get_sphere_uv(const point3_device& _p, double& _u, double& _v);

};


#endif
