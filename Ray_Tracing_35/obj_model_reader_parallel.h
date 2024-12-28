#ifndef OBJ_MODEL_READER_PARALLEL_H
#define OBJ_MODEL_READER_PARALLEL_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "color.h"
#include "material.h"
#include "parallel.h"
#include "hittable_list.h"
#include "hittable_list_parallel.h"
#include "vec3.h"
#include "triangle_mesh.h"
#include "obj_model_reader.h"
#include "mesh.h"


class obj_model_reader_parallel: public obj_model_reader 
{
public:
	obj_model_reader_parallel(std::string _obj_file_name, hittable_list* _world, parallel* _para);
	void reader();

protected:
	hittable_list* world;

	void add_item() override;

private:
	parallel* para;
};


#endif
