#include "sphere_gpu_openmp.h"


sphere_gpu_openmp::sphere_gpu_openmp(const sphere& _shape, const int& _mat_id):
    material_id(_mat_id)
{
    ray _center;
    double _radius;
    std::shared_ptr<material> _mat;
    _shape.return_params(_center, _radius, _mat);

    this->center = _center;
    this->radius = _radius;
}

bool sphere_gpu_openmp::hit(const ray& r, interval ray_t, hit_record& red) const {
    std::cerr << "The sphere_simple class is made specifically for "
        << "the use with gpu " << std::endl << " Please use the sphere class instead" << std::endl;
}

bool sphere_gpu_openmp::hit(const ray& r, interval ray_t, hit_record_gpu_openmp& rec) const {
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
    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}


void sphere_gpu_openmp::get_sphere_uv(const point3& p, double& u, double& v)
{

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
