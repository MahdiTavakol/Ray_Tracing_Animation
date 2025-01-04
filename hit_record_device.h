#ifndef HIT_REC_DEVICE_H
#define HIT_REC_DEVICE_H

#include "rtweekend.h"
#include "ray_device.h"
#include "vec3_device.h"
#include <memory>


class hit_record_device
{
public:
    point3_device p;
    vec3_device normal;
    double t;
    double u;
    double v;
    double w;
    bool front_face;

    void set_face_normal(const ray_device& r, const vec3_device& outward_normal);
};

#endif