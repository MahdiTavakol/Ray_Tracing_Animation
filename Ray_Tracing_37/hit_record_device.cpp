#include "hit_record_device.h"

__device__ void hit_record_device::set_face_normal(const ray_device& r, const vec3_device& outward_normal)
{
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}