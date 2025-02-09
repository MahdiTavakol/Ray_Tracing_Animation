#ifndef RENDERER_H
#define RENDERER_H

#include "rtweekend.h"

#include "obj_model_reader.h"
#include "obj_model_reader_parallel.h"
#include "aabb.h"
#include "bvh.h"
#include "camera.h"
#include "camera_derived.h"
#include "camera_parallel.h"
#include "color.h"
#include "color_array.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "input.h"
#include "interval.h"
#include "material.h"
#include "path.h"
#include "parallel.h"
#include "parallel_camera.h"
#include "parallel_derived.h"
#include "quad.h"
#include "ray.h"
#include "rtw_stb_image.h"
#include "planes.h"
#include "perlin.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"
#include "write.h"


class renderer
{
public:
	renderer(int argc, char** argv, int _mode, std::string _filename);
	~renderer();
	void setup();
	void add(shared_ptr<hittable> object);
	void render();
	void gather();
	void write_file();
	parallel* para_ptr() const;
	camera* cam_ptr() const;
	int return_num_frames() const;
	void move_camera(const int frame_i);
	void next_file(const int frame_i);

private:
	int mode;
	std::string filename;
	camera* cam;
	input* in;
	hittable_list* world;
	parallel* para;
	color_array* c_array, * c_array_all;
	write* writer;
	path* pth;
	


	void setup_random_spheres();
	void setup_checker_boards();
	void setup_earth_sphere();
	void setup_perlin_sphere();
	void setup_quads();
	void setup_simple_light();
	void setup_two_lights();
	void setup_cornell_box();
	void setup_boxes();
	void setup_boxes_rotated();
	void setup_cornell_smoke();
	void setup_3d_obj();
	void setup_3d_obj_parallel();
	void setup_random_spheres_animated();
	void setup_random_spheres_gpu();

};

#endif