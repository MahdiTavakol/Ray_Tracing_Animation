#include <fstream>

#include "rtweekend.h"

#include "camera.h"

#include "color.h"
#include "color_array.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"


void camera::render(const hittable& world, color_array& c_a) {
	initialize();



	for (int j = 0; j < image_height; j++)
	{
		std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world);
			}
			pixel_color = pixel_samples_scale * pixel_color;
			color_data** c_data = c_a.return_array();
			c_data[i][j].r = pixel_color.x();
			c_data[i][j].g = pixel_color.y();
			c_data[i][j].b = pixel_color.z();
		}
	}

	std::clog << "\rDone                          ";
}

void camera::initialize()
{
	image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	pixel_samples_scale = 1.0 / samples_per_pixel;

	center = lookfrom;

	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta / 2.0);
	auto viewport_height = 2 * h * focus_dist;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;

}

ray camera::get_ray(int i, int j) const
{
	auto offset = sample_square();
	auto pixel_sample = pixel00_loc
		+ ((i + offset.x()) * pixel_delta_u)
		+ ((j + offset.y()) * pixel_delta_v);

	auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;
	auto ray_time = random_double();

	return ray(ray_origin, ray_direction, ray_time);
}

vec3 camera::sample_square() const
{
	return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 camera::defocus_disk_sample() const
{
	auto p = random_in_unit_disk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const
{
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;

	if (world.hit(r, interval(0.001, infinity), rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, depth - 1, world);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

