#include <string>
#include <sstream>
#include <iostream>
#include "input.h"


input::input(int argc, char ** argv):image_width(1920), samples_per_pixel(100), max_depth(50), 
vfov(20), width_ratio(16.0), height_ratio(9.0), fps(60), num_seconds(10)
{
	int iarg = 0;
	while (iarg < argc)
	{
		if (argv[iarg] == "-image_width" || argv[iarg] == "--w")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->image_width = _int;
			iarg += 2;
		}
		if (argv[iarg] == "-samples_per_pixel" || argv[iarg] == "--s")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->samples_per_pixel = _int;
			iarg += 2;
		}
		if (argv[iarg] == "-max_depth" || argv[iarg] == "--d")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->max_depth = _int;
			iarg += 2;
		}
		if (argv[iarg] == "-vfov" || argv[iarg] == "--v")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->max_depth = _int;
			iarg += 2;
		}
		if (argv[iarg] == "-aspect_ratio" || argv[iarg] == "--a")
		{
			if (iarg + 2 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			double _double = convert_char<double>(argv[iarg + 1]);
			this->width_ratio = _double;
			_double = convert_char<double>(argv[iarg + 2]);
			this->height_ratio = _double;
			iarg += 3;
		}
		if (argv[iarg] == "-fps" || argv[iarg] == "--f")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			double _int = convert_char<int>(argv[iarg + 1]);
			this->fps = _int;
			iarg += 1;
		}
		if (argv[iarg] == "-num_seconds" || argv[iarg] == "--t")
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			double _int = convert_char<int>(argv[iarg + 1]);
			this->fps = _int;
			iarg += 1;
		}
	}


	this->lookfrom = point3(13, 2, 3);
	this->lookat = point3(0, 0, 0);
	this->vup = point3(0, 1, 0);


	this->defocus_angle = 0.6;
	this->focus_dist = 10.0;
}

void input::setup_camera(camera* cam) const
{
	cam->aspect_ratio = this->width_ratio / this->height_ratio;
	cam->image_width = this->image_width;
	cam->samples_per_pixel = this->samples_per_pixel;
	cam->max_depth = this->max_depth;
	//cam.vfov = 90;
	cam->vfov = this->vfov;
	cam->lookfrom = this->lookfrom;
	cam->lookat = this->lookat;
	cam->vup = this->vup;
	cam->defocus_angle = this->defocus_angle;
	cam->focus_dist = this->focus_dist;

	cam->initialize();
}

template<typename T>
T input::convert_char(char* _chr)
{
	T _t(0);
	std::string _string(_chr);
	std::stringstream iss(_string);

	if (_chr == nullptr)
	{
		throw std::invalid_argument("Null point passed to char_to_int");
	}

	iss >> _t;

	// Check if the conversion was successful
	if (iss.fail()) {
		throw std::invalid_argument("Conversion failed in char_to_int");
	}

	return _t;
}