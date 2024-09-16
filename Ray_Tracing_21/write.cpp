#include "write.h"

#include <fstream>
#include "color_array.h"

write::write(std::ofstream* _file, color_array* _colors, int _image_width, int _image_height) : 
	file(_file), colors(_colors), image_width(_image_width), image_height(_image_height) {}
	
write::~write()
{
	file->close();
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


