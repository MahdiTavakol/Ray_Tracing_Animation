#ifndef VEC3_DEVICE_H
#define VEC3_DEVICE_H

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "rtweekend.h"
#include "rtweekend_gpu.h"

class vec3_device {
public:
    double e[3];

    __device__ vec3_device() : e{ 0, 0, 0 } {}
    __device__ vec3_device(double _e0, double _e1, double _e2) : e{ _e0, _e1, _e2 } {}
    __device__ vec3_device(const vec3_device& _vc) : vec3_device(_vc[0], _vc[1], _vc[2]) {}

    __device__ double x() const { return e[0]; }
    __device__ double y() const { return e[1]; }
    __device__ double z() const { return e[2]; }

    __device__ vec3_device operator-() const { return vec3_device(-e[0], -e[1], -e[2]); }
    __device__ double operator[](int i) const { return e[i]; }
    __device__ double& operator[](int i) { return e[i]; }

    __device__ vec3_device& operator*=(double t);
    __device__ vec3_device& operator+=(vec3_device v);
    __device__ vec3_device& operator/=(double t);

    __device__ double length() const;
    __device__ double length_squared() const;
    __device__ bool near_zero() const;

    static __device__ vec3_device random();
    static __device__ vec3_device random(double min, double max);
};

using point3_device = vec3_device;

__device__ inline std::ostream& operator<<(std::ostream& out, const vec3_device& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

__device__ inline std::istream& operator>>(std::istream& in, vec3_device& v) {
    return in >> v.e[0] >> v.e[1] >> v.e[2];
}

__device__ inline vec3_device operator+(const vec3_device& u, const vec3_device& v) {
    return vec3_device(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

__device__ inline vec3_device operator-(const vec3_device& u, const vec3_device& v) {
    return vec3_device(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

__device__ inline vec3_device operator*(const vec3_device& u, const vec3_device& v) {
    return vec3_device(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

__device__ inline vec3_device operator*(double t, const vec3_device& v) {
    return vec3_device(t * v.e[0], t * v.e[1], t * v.e[2]);
}

__device__ inline vec3_device operator*(const vec3_device& v, double t) {
    return t * v;
}

__device__ inline vec3_device operator/(const vec3_device& v, double t) {
    return (1 / t) * v;
}

__device__ inline double dot(const vec3_device& u, const vec3_device& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

__device__ inline vec3_device cross(const vec3_device& u, const vec3_device& v) {
    return vec3_device(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

__device__ inline vec3_device unit_vector(const vec3_device& v) {
    return v / v.length();
}

/*inline static std::istream& operator>>(std::istream& cin, vec3_device& point)
{
    double x, y, z;
    if (cin >> x >> y >> z)
    {
        point = vec3_device(x, y, z);
    }
    else
    {
        cin.setstate(std::ios::failbit);
    }
    return cin;
}*/




__device__ vec3_device random_in_unit_disk();
__device__ vec3_device random_in_unit_sphere();
__device__ vec3_device random_unit_vector();
__device__ vec3_device random_on_hemisphere(const vec3_device& normal);
__device__ vec3_device reflect(const vec3_device& v, const vec3_device& n);
__device__ vec3_device refract(const vec3_device& uv, const vec3_device& n, double etai_over_etat);

#endif
