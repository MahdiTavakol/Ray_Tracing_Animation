#ifndef MATERIAL_DEVICE_H
#define MATERIAL_DEVICE_H


#include "color_device.h"
#include "hit_record_device.h"
#include "ray_device.h"
#include "vec3_device.h"

class metal_device
{
public:
	__device__ metal_device(const color_device& _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz) {}

	__device__ color emitted(double _u, double _v, const point3_device& _p) const
	{
		return color(0, 0, 0);
	}

	__device__ bool scatter(const ray_device& r_in, const hit_record_device& rec, color_device& attenuation, ray_device& scattered) const
	{
		vec3_device reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + fuzz * random_unit_vector();
		scattered = ray_device(rec.p, reflected, r_in.time());
		attenuation = albedo;
		return true;
	}


protected:
	color_device albedo;
	double fuzz;

};


class dielectric_device
{
public:
	__device__ dielectric_device(double _refraction_index) : refraction_index(_refraction_index) {}

	__device__ color emitted(double _u, double _v, const point3_device& _p) const
	{
		return color(0, 0, 0);
	}

	__device__ bool scatter(const ray_device& r_in, const hit_record_device& rec, color_device& attenuation, ray_device& scattered) const {
		attenuation = color_device(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3_device unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3_device direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray_device(rec.p, direction, r_in.time());
		return true;
	}



protected:
	double refraction_index;

	__device__ static double reflectance(double cosine, double refraction_index)
	{
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};



#endif