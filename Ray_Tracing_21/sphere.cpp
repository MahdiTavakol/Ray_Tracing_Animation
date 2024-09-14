#include "sphere.h"

sphere::sphere(const point3& _center, double _radius, std::shared_ptr<material> _mat)
    : center(_center, vec3(0, 0, 0)), radius(std::fmax(0, _radius)), mat(_mat) {}

sphere::sphere(const point3& _center1, const point3& _center2, double _radius, std::shared_ptr<material> _mat)
    : center(_center1, _center2 - _center1), radius(std::fmax(0, _radius)), mat(_mat) {}

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    point3 current_center = center.at(r.time());
    vec3 oc = current_center - r.origin();
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
    vec3 outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
}
