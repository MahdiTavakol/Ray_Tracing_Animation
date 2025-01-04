#include "vec3_device.h"
#include "rtweekend.h"

__device__ vec3_device& vec3_device::operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

__device__ vec3_device& vec3_device::operator+=(vec3_device v) {
    e[0] += v[0];
    e[1] += v[1];
    e[2] += v[2];
    return *this;
}

__device__ vec3_device& vec3_device::operator/=(double t) {
    if (t == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return *this *= 1 / t;
}

__device__ double vec3_device::length() const {
    return std::sqrt(length_squared());
}

__device__ double vec3_device::length_squared() const {
    return pow(e[0], 2) + pow(e[1], 2) + pow(e[2], 2);
}

__device__ bool vec3_device::near_zero() const {
    auto s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}

__device__ vec3_device vec3_device::random() {
    return vec3_device(random_double(), random_double(), random_double());
}

__device__ vec3_device vec3_device::random(double min, double max) {
    return vec3_device(random_double(min, max), random_double(min, max), random_double(min, max));
}

__device__ vec3_device random_in_unit_disk() {
    while (true) {
        auto p = vec3_device(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

__device__ vec3_device random_in_unit_sphere() {
    while (true) {
        auto p = vec3_device::random(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

__device__ vec3_device random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

__device__ vec3_device random_on_hemisphere(const vec3_device& normal) {
    vec3_device on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

__device__ vec3_device reflect(const vec3_device& v, const vec3_device& n) {
    return v - 2 * dot(v, n) * n;
}

__device__ vec3_device refract(const vec3_device& uv, const vec3_device& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3_device r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3_device r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

__device__ static void read_vec3_device(std::istream& cin, vec3_device& point)
{
    double x, y, z;
    cin >> x >> y >> z;
    point = vec3_device(x, y, z);
}

