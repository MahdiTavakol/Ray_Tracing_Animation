#ifndef PARALLEL_CAMERA_H
#define PARALLEL_CAMERA_H

#include "parallel.h"

class parallel_camera : public parallel
{
public:
	parallel_camera(hittable_list* _world, camera* _cam);
	void setup() override;
	void render() override;
	void gather() override;
	color_array* const color_array_ptr() override;
protected:
	int width_per_node, height_per_node;
	int width_min, width_max, height_min, height_max;
};



#endif