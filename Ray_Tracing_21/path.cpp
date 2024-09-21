#include <fstream>
#include <sstream>


#include "path.h"


path::path(std::string _filename, int _num_seconds, int _fps): filename(_filename), num_seconds(_num_seconds), fps(_fps)
{
	num_frames = num_seconds * fps;
	read_file();
	interpolate_points();
}

path::path(point3& _center, double& _radius, int& _num_seconds, int& _fps, double& theta): num_seconds(_num_seconds), fps(_fps)
{
	double theta_radians = pi * theta / 180.0;
	num_frames = num_seconds * fps;
	double speed = theta_radians / num_frames;

	for (int i = 0; i < num_frames; i++)
	{
		double second = i * fps;
		double _x = _center.x() - _radius * sin(static_cast<double>(i) * speed);
		double _y = _center.y();
		double _z = _center.z() + _radius * cos(static_cast<double>(i) * speed);
		point3_animated _loc(_x, _y, _z,second);
		locs.push_back(_loc);
	}
}

path::path(point3& _point)
{
	point3_animated _loc(_point, 0);
	init_locs.push_back(_loc);
}

void path::read_file()
{
	std::string line;
	double second;
	vec3 point;
	std::fstream file(filename);
	num_init_frames = 0;

	if (!file.is_open())
	{
		std::cerr << "Cannot open file " << filename << std::endl;
	}

	while (std::getline(file, line))
	{
		std::stringstream iss(line);
		if (iss >> second >> point)
		{
			std::clog << "Reading the second " << second << " from the file " << filename << std::endl;
			point3_animated _loc(point,second);
			init_locs.push_back(_loc);
			num_init_frames++;
		}
		else
		{
			std::cerr << "The format of the path file should be \"second x y z\" " << std::endl;
		}
	}
	std::clog << "Finished reading the file " << filename << std::endl;
	file.close();
}

void path::interpolate_points()
{
	int i = 0;  // counter for frames
	int j = 0;  // counter for init_seconds

	for (; i < num_frames; i++)
	{
		vec3 point;

		double time = static_cast<double>(i);
		double init_time = init_locs[j].return_time();

		// Finding the first frame in the input file that is bigger than the frame i to be used in the interpolation 
		while (init_time <= time)
		{
			if (std::abs(init_time-time) < 0.001)
			{
				point3_animated _loc(init_locs[i]);
				locs.push_back(_loc);
				continue;
			}
			j++;
			if (j < num_init_frames)
				init_time = init_locs[j].return_time();
			else
				break;
		}
		if (j == num_init_frames) j-=2;
		point3_animated init_loc_a = init_locs[j - 1];
		point3_animated init_loc_b = init_locs[j];
		point3_animated _loc = interpolate(init_loc_a, init_loc_b, time);
		locs.push_back(_loc);
	}
}