#include "sphere_device.h"

__device__ sphere_device::sphere_device(const point3_device& _center, double _radius)
    : center(_center, vec3_device(0, 0, 0)), radius(std::fmax(0, _radius)) {}

__device__ sphere_device::sphere_device(const point3_device& _center1, const point3_device& _center2, double _radius)
    : center(_center1, _center2 - _center1), radius(std::fmax(0, _radius)) {}

__device__ sphere_device::sphere_device(const sphere_device& _sphere)
    : center(_sphere.center), radius(_sphere.radius) {}

__device__ void sphere_device::hit(ray_device& _ray, hit_record_device& _rec, bool& _hit) const 
{
    point3_device current_center = center.at(_ray.time());
    vec3_device oc = current_center - _ray.origin();
    auto a = _ray.direction().length_squared();
    auto h = dot(_ray.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        _hit = false;
        return;
    }

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!_ray.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            _hit = false;
            return;
        }
    }

    _rec.t = root;
    _rec.p = _ray.at(_rec.t);
    vec3_device outward_normal = (_rec.p - current_center) / radius;
    _rec.set_face_normal(_ray, outward_normal);
    get_sphere_uv(outward_normal, _rec.u, _rec.v);

    _hit = true;
    return;
}

__device__ void sphere_device::get_sphere_uv(const point3_device& p, double& u, double& v)
{

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
