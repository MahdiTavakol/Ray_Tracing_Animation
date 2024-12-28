#include "hittable.h"

void hit_record::set_face_normal(const ray & r, const vec3 & outward_normal)
{
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

translate::translate(shared_ptr<hittable> _object, const vec3& _offset)
    :object(_object), offset(_offset)
{
    bbox = object->bounding_box() + offset;
}

bool translate::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
    ray offset_r(_r.origin() - offset, _r.direction(), _r.time());

    if (!object->hit(offset_r, _ray_t, _rec))
        return false;

    _rec.p += offset;

    return true;
}

rotate_y::rotate_y(shared_ptr<hittable> _object, double _angle) : object(_object)
{
    double radians = degrees_to_radians(_angle);
    sin_theta = std::sin(radians);
    cos_theta = std::cos(radians);

    bbox = _object->bounding_box();

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                double x = i * bbox.x.max + (1 - i) * bbox.x.min;
                double y = j * bbox.y.max + (1 - j) * bbox.y.min;
                double z = k * bbox.z.max + (1 - k) * bbox.z.min;

                double newx = cos_theta * x + sin_theta * z;
                double newz = -sin_theta * x + cos_theta * z;

                vec3 tester(newx, y, newz);
                for (int c = 0; c < 3; c++)
                {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
    bbox = aabb(min, max);
}


bool rotate_y::hit(const ray& _r, interval _ray_t, hit_record& _rec) const 
{
    double x_origin = (cos_theta * _r.origin().x()) - (sin_theta * _r.origin().z());
    double y_origin = _r.origin().y();
    double z_origin = (sin_theta * _r.origin().x()) + (cos_theta * _r.origin().z());
    double x_dir = cos_theta * _r.direction().x() - sin_theta * _r.direction().z();
    double y_dir = _r.direction().y();
    double z_dir = sin_theta * _r.direction().x() + cos_theta * _r.direction().z();
    point3 origin = point3(x_origin, y_origin, z_origin);
    vec3 direction = vec3(x_dir, y_dir, z_dir);

    ray rotated_r(origin, direction, _r.time());

    if (!object->hit(rotated_r, _ray_t, _rec))
        return false;

    double x_rec_p = cos_theta * _rec.p.x() + sin_theta * _rec.p.z();
    double y_rec_p = _rec.p.y();
    double z_rec_p = -sin_theta * _rec.p.x() + cos_theta * _rec.p.z();
    double x_rec_n = cos_theta * _rec.normal.x() + sin_theta * _rec.normal.z();
    double y_rec_n = _rec.normal.y();
    double z_rec_n = -sin_theta * _rec.normal.x() + cos_theta * _rec.normal.z();

    _rec.p = point3(x_rec_p, y_rec_p, z_rec_p);
    _rec.normal = vec3(x_rec_n, y_rec_n, z_rec_n);

    return true;
}