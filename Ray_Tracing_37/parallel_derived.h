#ifndef PARALLEL_DERIVED_H
#define PARALLEL_DERIVED_H

#include "parallel.h"

class parallel_derived : public parallel
{
public:
	parallel_derived(hittable_list* _world, camera* _cam);
	void setup() override;
	void render() override;
	void gather() override;
	color_array* const color_array_ptr() override;

};

#endif