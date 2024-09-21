#ifndef INPUT_H
#define INPUT_H

#include "camera.h"

class input
{
public:
	input(int argc, char** argv);
	const int& get_image_width() const
	{
		return this->image_width;
	}
	const int& get_samples_per_pixel() const
	{
		return this->samples_per_pixel;
	}
	const int& get_max_depth() const
	{
		return this->max_depth;
	}
	const int& get_vfov() const
	{
		return this->vfov;
	}
	void fps_num_seconds(int& _fps, int& _num_seconds) const
	{
		_fps = this->fps;
		_num_seconds = this->num_seconds;
	}
	void setup_camera(camera* cam) const;

private:
	template<typename T>
	T convert_char(char* _chr);
	int image_width;
	int samples_per_pixel;
	int max_depth;
	int vfov;
	double width_ratio;
    double height_ratio;
	double defocus_angle;
	double focus_dist;

	point3 lookfrom;
	point3 lookat;
	point3 vup;

	int fps;
	int num_seconds;
};

#endif