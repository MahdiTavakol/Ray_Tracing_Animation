#ifndef OBJ_MODEL_READER_H
#define OBJ_MODEL_READER_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "color.h"
#include "material.h"
#include "hittable_list.h"
#include "vec3.h"
#include "triangle_mesh.h"
#include "mesh.h"

struct face_indx
{
	std::vector<int> v_indx;
	std::vector<int> vt_indx;
	std::vector<int> vn_indx;
	int num_edges;
	int mat_indx;
};

class obj_model_reader {
public:
	obj_model_reader() {}
	obj_model_reader(std::string _obj_file_name, hittable_list* _world);
	void reader();

protected:
	hittable_list* world;
	std::string obj_file_name, mtl_file_name;
	std::vector<vec3> vs, vts, vns;
	int v_num, vt_num, vn_num;
	int polygon_num, triangle_num;
	std::vector<std::string> mat_names;
	std::vector<face_indx> face_indexes;
	std::vector<shared_ptr<material>> materials;

	void read_obj_file();
	void read_mtl_file();
	virtual void add_item();
};


#endif
