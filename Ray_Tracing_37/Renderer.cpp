#include "Renderer.h"


renderer::renderer(int argc, char** argv, int _mode, std::string _filename)
	: mode(_mode), filename(_filename), c_array(nullptr), c_array_all(nullptr),
 	  world(nullptr), world_parallel(nullptr),
	  cam(nullptr),cam_derived(nullptr), pth(nullptr)
{
	in = new input(argc, argv, mode);

	switch (mode)
	{
		case OBJ_MODEL_PARALLEL:
			cam_derived = new camera_derived(in);
			world_parallel = new hittable_list_parallel();
			para = new parallel(world_parallel, cam_derived);
			break;
		default:
			cam = new camera_parallel(in);
			world = new hittable_list();
			para = new parallel(world, cam);
			break;
	}

	
	writer = new write(filename);

	// The default value
	pth = new path(point3(0, 0, 12));
}

renderer::~renderer()
{
	delete in;
	if (cam) delete cam;
	if (cam_derived) delete cam_derived;
	if (world) delete world;
	if (world_parallel) delete world_parallel;
	if (para) delete para;
	if (writer) delete writer;
	if (pth) delete pth;
	c_array = nullptr;
	c_array_all = nullptr;
}

void renderer::setup()
{
	c_array = para->color_array_ptr();

	switch (mode)
	{
		case RANDOM_SPHERES:
			setup_random_spheres();
			break;
		case CHECKER_BOARDS:
			setup_checker_boards();
			break;
		case EARTH_SPHERE:
			setup_earth_sphere();
			break;
		case PERLIN_SPHERE:
			setup_perlin_sphere();
			break;
		case QUADS:
			setup_quads();
			break;
		case SIMPLE_LIGHT:
			setup_simple_light();
			break;
		case TWO_LIGHTS:
			setup_two_lights();
			break;
		case CORNELL_BOX:
			setup_cornell_box();
			break;
		case TWO_BOXES:
			setup_boxes();
			break;
		case TWO_BOXES_ROTATED:
			setup_boxes_rotated();
			break;
		case CORNELL_SMOKE:
			setup_cornell_smoke();
			break;
		case OBJ_MODEL:
			setup_3d_obj();
			break;
		case OBJ_MODEL_PARALLEL:
			setup_3d_obj_parallel();
			break;
		case RANDOM_SPHERES_ANIMATED:
			setup_random_spheres_animated();
			break;
	}

	para->setup();

}


void renderer::add(shared_ptr<hittable> object)
{
	world->add(object);
}

void renderer::render()
{
	//c_array = para->color_array_ptr(); // This points to the color_array object belonging to the parallel object.
	para->render();
}

void renderer::gather()
{
	if (cam)
		para->gather();
	c_array_all = para->color_array_all_ptr();
}

void renderer::write_file()
{
	int image_width, image_height;

	if (cam)
		cam->return_image_size(image_width, image_height);
	else if (cam_derived)
		cam_derived->return_image_size(image_width, image_height);
	writer->reset(c_array_all, image_width, image_height);

	int rank = para->return_rank();
	if (rank == 0) writer->write_file();
}

parallel* renderer::para_ptr() const {
	return para;
}

camera* renderer::cam_ptr() const
{
	if (mode == OBJ_MODEL_PARALLEL)
		return cam_derived;
	return cam;
}

int renderer::return_num_frames() const
{
	int fps, num_seconds, num_frames;
	in->fps_num_seconds(fps, num_seconds);
	num_frames = fps * num_seconds;
	return num_frames;
}

void renderer::move_camera(const int frame_i)
{
	// moving the camera
	if (cam)
		cam->move_camera((*pth)[frame_i]);
	if (cam_derived)
		cam_derived->move_camera((*pth)[frame_i]);
}

void renderer::next_file(const int frame_i)
{
	int fps, num_seconds, num_frames;
	in->fps_num_seconds(fps, num_seconds);
	num_frames = fps * num_seconds;
	// There is no next frames in this case!
	if (frame_i + 1 == num_frames) 
		return;
	std::string next_file_name = "frame-" + std::to_string(frame_i + 1) + ".ppm";
	writer->open_new_file(next_file_name);	
}

void renderer::setup_random_spheres()
{

	auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	world->add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					world->add(make_shared<sphere>(center, center2, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world->add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world->add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}

		}

	auto material1 = make_shared<dielectric>(1.5);
	world->add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world->add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world->add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
	
	*world = hittable_list(make_shared<bvh_node>(*world));
}

void renderer::setup_checker_boards()
{
	auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

	world->add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world->add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));
	
	*world = hittable_list(make_shared<bvh_node>(*world));
}

void renderer::setup_earth_sphere()
{
	auto earth_texture = make_shared<image_texture>("3840px-Blue_Marble_2002.png");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

	world->add(globe);

	*world = hittable_list(make_shared<bvh_node>(*world));
}

void renderer::setup_perlin_sphere()
{
	auto pertext = make_shared<noise_texture>(4);
	world->add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world->add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	*world = hittable_list(make_shared<bvh_node>(*world));

}

void renderer::setup_quads()
{
	auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
	auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

	world->add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world->add(make_shared<circle>(point3(-2, -2, 0), vec3(4, 0,  0), vec3(0, 4, 0), back_green));
	world->add(make_shared<triangle>(point3( 3, -2, 1), vec3(0, 0,  4), vec3(0, 4, 0), right_blue));
	world->add(make_shared<quad>(point3(-2,  3, 1), vec3(4, 0,  0), vec3(0, 0, 4), upper_orange));
	world->add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0,  0), vec3(0, 0,-4), lower_teal));



}

void renderer::setup_simple_light()
{
	*pth = path(point3(26, 3, 6));

	auto pertext = make_shared<noise_texture>(4);
	world->add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world->add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	world->add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));
}

void renderer::setup_two_lights()
{
	*pth = path(point3(26,3,6));

	this->setup_simple_light();
	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	world->add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
}

void renderer::setup_cornell_box()
{
	*pth = path(point3(278, 278, -800));
	shared_ptr<material> red = make_shared<lambertian>(color(0.64, 0.05, 0.05));
	shared_ptr<material> white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	shared_ptr<material> green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
	shared_ptr<material> light = make_shared<diffuse_light>(color(15,15,15));

	world->add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world->add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world->add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
	world->add(make_shared<quad>(point3(555, 555, 555), vec3(-555,0, 0), vec3(0, 0, -555), white));
	world->add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

}

void renderer::setup_boxes()
{
	setup_cornell_box();
	shared_ptr<material> white = make_shared<lambertian>(color(0.73, 0.73, 0.73));

	

	world->add(box(point3(130, 0, 65), point3(295, 165, 230), white));
	world->add(box(point3(265, 0, 295), point3(430, 330, 460), white));
}

void renderer::setup_boxes_rotated()
{
	setup_cornell_box();

	shared_ptr<material> white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	world->add(box1);

	shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	world->add(box2);

}

void renderer::setup_cornell_smoke()
{
	setup_cornell_box();

	shared_ptr<material> white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));


	shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	world->add(box1);
	world->add(box2);
	world->add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
	world->add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));;

}


void renderer::setup_3d_obj()
{
	std::string obj_file_name("Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj");
	shared_ptr<obj_model_reader> model_reader = make_shared<obj_model_reader>(obj_file_name, world);
	model_reader->reader();
}

void renderer::setup_3d_obj_parallel()
{
	std::string obj_file_name("Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj");
	shared_ptr<obj_model_reader_parallel> model_reader = make_shared<obj_model_reader_parallel>(obj_file_name, world_parallel, para);
	model_reader->reader();
}

void renderer::setup_random_spheres_animated()
{
	// The movie setting
	int fps, num_seconds;

	// Creating the trajectory of the camera
	point3 center(13, 2, 3);
	double radius = 13.49;
	double theta = 45.0;

	// Creating the spheres 
	setup_random_spheres();

	// Get the fps and num_frames
	in->fps_num_seconds(fps, num_seconds);

	// Creating a new path variable containing the camera locations
	*pth = path(center, radius, num_seconds, fps, theta);

}