#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "rtweekend.h"

class vec3 {
public:
    double e[3];

    vec3() : e{ 0, 0, 0 } {}
    vec3(double _e0, double _e1, double _e2) : e{ _e0, _e1, _e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator*=(double t);
    vec3& operator+=(vec3 v);
    vec3& operator/=(double t);

    double length() const;
    double length_squared() const;
    bool near_zero() const;

    static vec3 random();
    static vec3 random(double min, double max);
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline std::istream& operator>>(std::istream& in, vec3& v) {
    return in >> v.e[0] >> v.e[1] >> v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

/*inline static std::istream& operator>>(std::istream& cin, vec3& point)
{
    double x, y, z;
    if (cin >> x >> y >> z)
    {
        point = vec3(x, y, z);
    }
    else
    {
        cin.setstate(std::ios::failbit);
    }
    return cin;
}*/




vec3 random_in_unit_disk();
vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_on_hemisphere(const vec3& normal);
vec3 reflect(const vec3& v, const vec3& n);
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat);

#endif
