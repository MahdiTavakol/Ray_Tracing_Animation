#ifndef MATERIAL_H
#define MATERIAL_H


#include "rtweekend.h"

#include "ray.h"
#include "color.h"
#include "texture.h"
#include "hittable.h"

//class hit_record;

class material
{
public:
	virtual ~material() = default;

	virtual color emitted(double _u, double _v, const point3& _p) const
	{
		return color(0, 0, 0);
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
	{
		return false;
	}
};

class general : public material
{
public:
	general(const color& _albedo, const double& _shininess,
		const double& _d, const double& _Tr, const color& _Tf, const color _Ks) :
		albedo(_albedo), shininess(_shininess), d(_d), Tr(_Tr), Tf(_Tf), Ks(_Ks)
	{}


	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		// Calculate the effective transparency based on d (dissolve) and Tr (transparency)
		double effective_transparency = (1.0 - d) * Tr;

		// Reflect the incoming ray based on the hit normal
		vec3 reflected = reflect(r_in.direction(), rec.normal);

		// Make reflection a bit fuzzier based on `fuzz` parameter
		//reflected += fuzz * random_unit_vector(); // Adjusted to fuzziness

		// Control sharpness of reflection based on shininess (Ns)
		reflected = reflected * (1.0 - shininess / 100.0) + rec.normal * (shininess / 100.0);

		// Scatter the ray with the adjusted direction
		scattered = ray(rec.p, reflected, r_in.time());

		// Apply attenuation based on effective transparency
		// You may choose to mix the albedo and some transparency effect
		attenuation = albedo * (1.0 - effective_transparency) + Tf * effective_transparency;

		return true;
	}


private:
	color albedo;
	double shininess;
	double d, Tr;
	color Ka, Ks, Tf;
};


class lambertian : public material
{
public:
	lambertian(const color& _albedo) : tex(make_shared<solid_color>(_albedo)) {}
	lambertian(shared_ptr<texture> _tex) : tex(_tex) {}


	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = tex->value(rec.u, rec.v, rec.p);
		return true;
	}
private:
	shared_ptr<texture> tex;
};

class metal : public material
{
public:
	metal(const color& _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + fuzz * random_unit_vector();
		scattered = ray(rec.p, reflected, r_in.time());
		attenuation = albedo;
		return true;
	}
private:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double _refraction_index) : refraction_index(_refraction_index) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray(rec.p, direction, r_in.time());
		return true;
	}

private:
	double refraction_index;

	static double reflectance(double cosine, double refraction_index)
	{
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};


class diffuse_light : public material {
public:
	diffuse_light(shared_ptr<texture> _tex) : tex(_tex) {}
	diffuse_light(const color& _emit) : tex(make_shared<solid_color>(_emit)) {}

	color emitted(double _u, double _v, const point3& _p)  const override {
		return tex->value(_u, _v, _p);
	}

private:
	shared_ptr<texture> tex;
};

class isotropic : public material
{
public:
	isotropic(const color& _albedo) : tex(make_shared<solid_color>(_albedo)) {}
	isotropic(shared_ptr<texture> _tex) : tex(_tex) {}

	bool scatter(const ray& _r_in, const hit_record& _rec, color& _attenuation, ray& _scattered) const override
	{
		_scattered = ray(_rec.p, random_unit_vector(), _r_in.time());
		_attenuation = tex->value(_rec.u, _rec.v, _rec.p);
		return true;
	}
private:
	shared_ptr<texture> tex;
};


#endif