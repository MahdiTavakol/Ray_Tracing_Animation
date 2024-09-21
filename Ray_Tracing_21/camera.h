#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>

#include "rtweekend.h"


#include "color.h"
#include "hittable.h"
#include "hittable_list.h"

class color_array;
class input;


class camera {
public:
	friend class input;


	void render(const hittable& world, color_array& c_a);

	void move_camera(point3 _lookfrom) {
		this->lookfrom = _lookfrom;
	}

	void return_image_size(int& _image_width, int& _image_height) const
	{
		_image_width = this->image_width;
		_image_height = this->image_height;
	}

protected:

	// primary parameters set by the class input;
	double aspect_ratio = 1.0;
	int image_width = 100;
	int image_height;
	int samples_per_pixel = 10;
	int max_depth = 10;
	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);
	double defocus_angle = 0;
	double focus_dist = 10;




	// secondary parameters set by the initialize method
	double pixel_samples_scale;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;
	vec3 defocus_disk_u;
	vec3 defocus_disk_v;

	// the rest of functions
	ray get_ray(int i, int j) const;
	vec3 sample_square() const;
	point3 defocus_disk_sample() const;
	color ray_color(const ray& r, int depth, const hittable& world) const;
	void initialize();
};


#endif
