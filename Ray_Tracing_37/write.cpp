#include "write.h"

#include <fstream>
#include "color_array.h"

write::write(std::string _file_name, color_array* _colors, int _image_width, int _image_height) : 
	file_name(_file_name), colors(_colors), image_width(_image_width), image_height(_image_height)
{
	file = new std::ofstream(file_name);
}

write::write(std::string _file_name) : write(_file_name, nullptr, 0, 0) {}
	
write::~write()
{
	if (file->is_open())
	{
		file->close();
		if (file)
			delete file;
	}
}

void write::reset(color_array* _colors, const int _image_width, const int _image_height)
{
	this->colors = _colors;
	this->image_width = _image_width;
	this->image_height = _image_height;
}

void write::open_new_file(std::string _file_name)
{
	file_name = _file_name;
	if (file->is_open())
		file->close();
	file->open(file_name);
	if (!file->is_open())
		std::cerr << "Cannot open the file " << file_name << std::endl << "That is all we know at the moment!" << std::endl;
}

void write::write_file()
{
	*file << "P3\n" << image_width << " " << image_height << "\n255\n";
	color_data** c_data = colors->return_array();

	for (int j = 0; j < image_height; j++)
	{
		for (int i = 0; i < image_width; i++)
		{
			*file << c_data[i][j]; //This leads to strided access ==> might need to change the indexing to [height_index][width_index]
		}
	}
}


