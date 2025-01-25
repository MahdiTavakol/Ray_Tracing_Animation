#ifndef TEXTURE_DEVICE_H
#define TEXTURE_DEVICE_H

#include "color_device.h"
#include "rtw_stb_image.h"
#include "perlin_device.h"
#include "interval_device.h"

class texture_device
{
public:
	virtual ~texture_device() = default;

	virtual color_device value(double u, double v, const point3_device& p) const = 0;
};

class solid_color_device : public texture_device
{
public:
	solid_color_device(const color_device& _albedo) : albedo(_albedo) {}

	solid_color_device(double _red, double _green, double _blue) :
		solid_color_device(color_device(_red, _green, _blue)) {
	}

	color_device value(double _u, double _v, const point3_device& p) const override
	{
		return albedo;
	}

private:
	color_device albedo;
};

class checker_texture_device : public texture_device
{
public:
	checker_texture_device(double _scale, shared_ptr<texture_device> _even, shared_ptr<texture_device> _odd)
		: inv_scale(1.0 / _scale), even(_even), odd(_odd) {
	}

	checker_texture_device(double _scale, const color_device& _c1, const color_device& _c2)
		: checker_texture_device(_scale, make_shared<solid_color_device>(_c1), make_shared<solid_color_device>(_c2)) {
	}

	color_device value(double _u, double _v, const point3_device& _p) const override
	{
		auto xInteger = int(std::floor(inv_scale * _p.x()));
		auto yInteger = int(std::floor(inv_scale * _p.y()));
		auto zInteger = int(std::floor(inv_scale * _p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? even->value(_u, _v, _p) : odd->value(_u, _v, _p);
	}

private:
	double inv_scale;
	shared_ptr<texture_device> even;
	shared_ptr<texture_device> odd;
};

class image_texture_device : public texture_device
{
public:
	image_texture_device(const char* _filename) :image(_filename) {}

	color_device value(double u, double v, const point3_device& p) const override
	{
		if (image.height() <= 0) return color_device(0, 1, 1);

		u = interval_device(0, 1).clamp(u);
		v = 1.0 - interval_device(0, 1).clamp(v);

		auto i = int(u * image.width());
		auto j = int(v * image.height());
		auto pixel = image.pixel_data(i, j);

		auto color_scale = 1.0 / 255.0;
		return color_device(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}
private:
	rtw_image image;
};

class noise_texture_device : public texture_device
{
public:
	noise_texture_device(double _scale) : scale(_scale) {}

	color_device value(double _u, double _b, const point3_device& _p) const override
	{
		return color_device(0.5, 0.5, 0.5) * (1 + std::sin(scale * _p.z() + 10 * noise.turb(_p, 7)));
		//return color(1, 1, 1) * noise.turb(p, 7);
		//return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * _p));
	}
private:
	perlin_device noise;
	double scale;
};

#endif