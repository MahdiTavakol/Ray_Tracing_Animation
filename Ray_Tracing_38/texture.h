#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "rtw_stb_image.h"
#include "perlin.h"

class texture
{
public:
	virtual ~texture() = default;

	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
	solid_color(const color& _albedo) : albedo(_albedo) {}

	solid_color(double _red, double _green,double _blue): 
		solid_color(color(_red,_green,_blue)) {}

	color value(double _u, double _v, const point3& p) const override
	{
		return albedo;
	}

private:
	color albedo;
};

class checker_texture : public texture
{
public:
	checker_texture(double _scale, shared_ptr<texture> _even, shared_ptr<texture> _odd )
		: inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

	checker_texture(double _scale, const color& _c1, const color& _c2)
		: checker_texture(_scale, make_shared<solid_color>(_c1), make_shared<solid_color>(_c2)) {}

	color value(double _u, double _v, const point3& _p) const override
	{
		auto xInteger = int(std::floor(inv_scale * _p.x()));
		auto yInteger = int(std::floor(inv_scale * _p.y()));
		auto zInteger = int(std::floor(inv_scale * _p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? even->value(_u, _v, _p) : odd->value(_u, _v, _p);
	}

private:
	double inv_scale;
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};

class image_texture : public texture
{
public:
	image_texture(const char* _filename) :image(_filename) {}

	color value(double u, double v, const point3& p) const override
	{
		if (image.height() <= 0) return color(0, 1, 1);

		u = interval(0, 1).clamp(u);
		v = 1.0 - interval(0, 1).clamp(v);

		auto i = int(u * image.width());
		auto j = int(v * image.height());
		auto pixel = image.pixel_data(i, j);

		auto color_scale = 1.0 / 255.0;
		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}
private:
	rtw_image image;
};

class noise_texture : public texture
{
public:
	noise_texture(double _scale) : scale(_scale) {}

	color value(double _u, double _b, const point3& _p) const override
	{
		return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * _p.z() + 10 * noise.turb(_p, 7)));
		//return color(1, 1, 1) * noise.turb(p, 7);
		//return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * _p));
	}
private:
	perlin noise;
	double scale;
};

#endif