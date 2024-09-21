#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include "input.h"


input::input(int argc, char** argv) :image_width(1920), samples_per_pixel(100), max_depth(50),
vfov(20), width_ratio(16.0), height_ratio(9.0), fps(60), num_seconds(10), input_logger(false)
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
	this->focus_dist = 10.0;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) input_logger_function(argc, argv);
}

void input::input_logger_function(int argc, char** argv)
{
	logfile.open("RayTracingInput.log",std::ios::out);
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
