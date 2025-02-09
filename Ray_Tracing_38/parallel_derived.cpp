#include "parallel_derived.h"

parallel_derived::parallel_derived(hittable_list* _world, camera* _cam)
{
	camera_derived* cam_type_test = dynamic_cast<camera_derived*>(_cam);
	hittable_list_parallel* world_type_test = dynamic_cast<hittable_list_parallel*>(_world);

	if (!cam_type_test) std::cerr << "Wrong camera type" << std::endl;
	if (!world_type_test) std::cerr << "Wrong world type" << std::endl;
}

void parallel_derived::setup()
{
	c_array_all.reset_size(cam->image_width, cam->image_height);
}

void parallel_derived::render()
{
	cam->render(*world, c_array_all);
}

void parallel_derived::gather()
{
	std::cerr << "The gather method should in the parallel class should be implemented for a non-parallel camera object" << std::endl;
}

color_array* const parallel_derived::color_array_ptr()
{
	std::cerr << "Undefined behavior" << std::endl;
}