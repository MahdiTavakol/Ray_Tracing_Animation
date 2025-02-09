#ifndef HIT_RECORD_SIMPLE_H
#define HIT_RECORD_SIMPLE_H

#include "rtweekend.h"
#include "vec3.h"
#include "material.h"

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


#endif