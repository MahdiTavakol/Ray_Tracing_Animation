#ifndef WRITE_H
#define WRITE_H

#include <fstream>

#include "rtweekend.h"

#include "color_array.h"

class write {
public:
	write(std::string _file_name, color_array* _colors, int _image_width, int _image_height);
	write(std::string _file_name);
	~write();
	void write_file();
	void reset(color_array*_colors, const int _image_width, const int _image_height);
	void open_new_file(std::string _file_name);
private:
	std::string file_name;
	std::ofstream* file;
	color_array* colors;
	int image_width, image_height;
};




#endif