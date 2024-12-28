#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include "input.h"


input::input(int argc, char** argv, int mode):image_width(50), samples_per_pixel(150), max_depth(50),
vfov(20), width_ratio(16.0), height_ratio(9.0), fps(1), num_seconds(1), input_logger(false)
{
	int iarg = 1;
	while (iarg < argc)
	{
		if (!strcmp(argv[iarg], "-image_width") || !strcmp(argv[iarg], "--w"))
		{
			if (iarg + 1 < argc)
			{
				int _int = convert_char<int>(argv[iarg + 1]);
				this->image_width = _int;
				iarg += 2;
			}
			else
			{
				std::cerr << "Invalid input arguments" << std::endl;
			}
		}
		if (!strcmp(argv[iarg], "-samples_per_pixel") || !strcmp(argv[iarg], "--s"))
		{
			if (iarg + 1 < argc)
			{
				int _int = convert_char<int>(argv[iarg + 1]);
				this->samples_per_pixel = _int;
				iarg += 2;
			}
			else
			{

				std::cerr << "Invalid input arguments" << std::endl;
			}
		}
		if (!strcmp(argv[iarg], "-max_depth") || !strcmp(argv[iarg], "--d"))
		{
			if (iarg + 1 < argc)
			{
				int _int = convert_char<int>(argv[iarg + 1]);
				this->max_depth = _int;
				iarg += 2;
			}
			else
			{
				std::cerr << "Invalid input arguments" << std::endl;
			}
		}
		if (!strcmp(argv[iarg], "-vfov") || !strcmp(argv[iarg], "--v"))
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->vfov = _int;
			iarg += 2;
		}
		if (!strcmp(argv[iarg], "-aspect_ratio") || !strcmp(argv[iarg], "--a"))
		{
			if (iarg + 2 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			double _double = convert_char<double>(argv[iarg + 1]);
			this->width_ratio = _double;
			_double = convert_char<double>(argv[iarg + 2]);
			this->height_ratio = _double;
			iarg += 3;
		}
		if (!strcmp(argv[iarg], "-fps") || !strcmp(argv[iarg], "--f"))
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->fps = _int;
			iarg += 2;
		}
		if (!strcmp(argv[iarg], "-num_seconds") || !strcmp(argv[iarg], "--t"))
		{
			if (iarg + 1 >= argc)
				std::cerr << "Invalid input arguments" << std::endl;
			int _int = convert_char<int>(argv[iarg + 1]);
			this->num_seconds = _int;
			iarg += 2;
		}
		else
		{
			std::cerr << "Unknown command line argument " << argv[iarg] << std::endl;
		}
	}

	this->lookfrom = point3(13, 2, 3);
	this->lookat = point3(0, 0, 0);
	this->vup = point3(0, 1, 0);


	this->defocus_angle = 0.6;
	this->defocus_angle = 0;
	this->focus_dist = 10.0;
	this->background = color(0, 0, 0);
	

	if (mode == QUADS)
	{
		this->image_width = 1080;
		this->samples_per_pixel = 100;
		this->max_depth = 50;

		this->width_ratio = 1.0;
		this->height_ratio = 1.0;

		this->vfov = 80;
		this->lookfrom = point3(0, 0, 9);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);

		this->defocus_angle = 0;
	}
	else if (mode == TWO_LIGHTS || mode == SIMPLE_LIGHT)
	{
		this->lookfrom = point3(26, 3, 6);
		this->lookat = point3(0, 1, 0);
		this->defocus_angle = 0;
		this->vfov = 20;
		this->background = color(0, 0, 0);
	}
	else if (mode == CORNELL_BOX || mode == TWO_BOXES || mode == TWO_BOXES_ROTATED)
	{
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vfov = 40;
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		this->background = color(0, 0, 0);
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
	}
	else if (mode == CORNELL_SMOKE)
	{
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
		this->background = color(0, 0, 0);

		this->vfov = 40;
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
	}
	else if (mode == RANDOM_SPHERES_ANIMATED)
	{
		this->fps = 60.0;
		this->num_seconds = 10;
		this->background = color(0.7, 0.8, 1.00);
		// The rest of the thing
	}
	else this->background = color(0.7, 0.8, 1.00);
	


	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) input_logger_function(argc, argv);

}


input::input(int argc, char** argv, int _image_width, int _samples_per_pixel,
	int _max_depth, int _vfov, double _width_ratio, double _height_ratio)
	: image_width(_image_width), samples_per_pixel(_samples_per_pixel), max_depth(_max_depth),
	vfov(_vfov), width_ratio(_width_ratio), height_ratio(_height_ratio),
	fps(60), num_seconds(10), input_logger(false)

{
	this->lookfrom = point3(13, 2, 3);
	this->lookat = point3(0, 0, 0);
	this->vup = point3(0, 1, 0);


	this->defocus_angle = 0.6;
	this->defocus_angle = 0;
	this->focus_dist = 10.0;

	this->background = color(0.7, 0.8, 1.00);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) input_logger_function(argc, argv);
}


void input::input_logger_function(int argc, char** argv)
{
	logfile.open("RayTracingInput.log", std::ios::out);
	if (!logfile.is_open())
		std::cerr << "Cannot open the RayTracing.log file for logging" << std::endl;
	else
	{
		logfile << "The input: " << std::endl;
		for (int i = 0; i < argc; i++)
			logfile << argv[i] << " ";
		logfile << std::endl << "The parsed values: " << std::endl;

		logfile << argv[0] << std::endl;
		logfile << "image_width = " << this->image_width << std::endl;
		logfile << "samples_per_pixel = " << this->samples_per_pixel << std::endl;
		logfile << "max_depth = " << this->max_depth << std::endl;
		logfile << "vfov = " << this->vfov << std::endl;
		logfile << "aspect_ratio = " << this->width_ratio << "/" << this->height_ratio << std::endl;
		logfile << "fps = " << this->fps << std::endl;
		logfile << "num_seconds = " << this->num_seconds << std::endl;
	}
	logfile.close();

	std::cout << std::endl;
	std::cout << "Start making frames using with the following properties:" << std::endl;
	std::cout << "image_width = " << this->image_width << std::endl;
	std::cout << "samples_per_pixel = " << this->samples_per_pixel << std::endl;
	std::cout << "max_depth = " << this->max_depth << std::endl;
	std::cout << "vfov = " << this->vfov << std::endl;
	std::cout << "aspect_ratio = " << this->width_ratio << "/" << this->height_ratio << std::endl;
	std::cout << "fps = " << this->fps << std::endl;
	std::cout << "num_seconds = " << this->num_seconds << std::endl;
	std::cout << std::endl;
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
	cam->background = this->background;

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