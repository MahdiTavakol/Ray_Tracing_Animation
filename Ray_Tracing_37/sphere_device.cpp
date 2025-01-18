#include "sphere_device.h"

__device__ sphere_device::sphere_device(const point3_device& _center, double _radius)
    : center(_center, vec3_device(0, 0, 0)), radius(std::fmax(0, _radius)) {}

__device__ sphere_device::sphere_device(const point3_device& _center1, const point3_device& _center2, double _radius)
    : center(_center1, _center2 - _center1), radius(std::fmax(0, _radius)) {}

__device__ sphere_device::sphere_device(const ray_device& _center, double _radius)
    : center(_center), radius(_radius) {}

__device__ sphere_device::sphere_device(const sphere_device& _sphere)
    : center(_sphere.center), radius(_sphere.radius) {}

__device__ bool sphere_device::hit(const ray_device& r, interval_device ray_t, hit_record_device& rec) const
{
    point3_device current_center = center.at(r.time());
    vec3_device oc = current_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3_device outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
}

__device__ void sphere_device::get_sphere_uv(const point3_device& p, double& u, double& v)
{

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
