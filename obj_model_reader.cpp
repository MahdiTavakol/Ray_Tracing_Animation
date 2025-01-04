#include "obj_model_reader.h"


obj_model_reader::obj_model_reader(std::string _obj_file_name, hittable_list* _world) :
	obj_file_name(_obj_file_name), v_num(0), vt_num(0), vn_num(0), world(_world) {}

void obj_model_reader::reader()
{
	std::cout << "Started reading the obj file " << std::endl;
	read_obj_file();
	std::cout << "Finished reading the obj file " << std::endl;
	std::cout << "Started reading the mtl file " << std::endl;
	read_mtl_file();
	std::cout << "Finished reading the mtl file " << std::endl;
	std::cout << "Started adding the items " << std::endl;
	add_item();
}

void obj_model_reader::read_obj_file()
{
	std::string line;
	std::ifstream obj_file(obj_file_name);
	std::stringstream iss;

	int file_vs_num = 0, file_vts_num = 0, file_vns_num = 0;
	int file_polygon_num = 0, file_triangle_num = 0;

	face_indx faces;
	int num_edges = 0;
	int num_polygons = 0;
	int num_triangles = 0;
	int current_mat_indx = -1;

	while (std::getline(obj_file, line))
	{
		iss.clear();
		iss.str("");
		int dummy_int;
		std::string dummy_str;
		vec3 point3;
		if (line.length() >= 3)
		{
			iss << line;
			iss >> dummy_str;
			if (!dummy_str.compare("#"))
			{
				if (iss >> dummy_int >> dummy_str)
				{
					if (dummy_str == "vertices")
						file_vs_num = dummy_int;
					if (dummy_str == "vertex")
						file_vns_num = dummy_int;
					if (dummy_str == "texture")
						file_vts_num = dummy_int;
					if (dummy_str == "polygons")
					{
						file_polygon_num = dummy_int;
						if (iss >> dummy_str >> dummy_int >> dummy_str)
						{
							if (dummy_str == "triangles")
								file_triangle_num = dummy_int;
						}
					}
				}
			}
			if (!dummy_str.compare("mtllib"))
			{
				iss >> dummy_str;
				mtl_file_name = std::string(dummy_str);
			}
			if (!dummy_str.compare("o"))
			{
				iss >> dummy_str;
			}
			if (!dummy_str.compare("g"))
			{
				iss >> dummy_str;
			}
			if (!dummy_str.compare("usemtl"))
			{
				iss >> dummy_str;
				mat_names.push_back(dummy_str);
				current_mat_indx++;
			}
			if (!dummy_str.compare("s"))
			{
				// I do not know what to do with this!
			}
			else if (!dummy_str.compare("f"))
			{
				face_indx face;
				int num_edges = 0;
				while (iss >> dummy_str)
				{
					int v_indx, vt_indx, vn_indx;
					char delimiter = '/';
					std::istringstream issr(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					v_indx = std::stoi(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					vt_indx = std::stoi(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					vn_indx = std::stoi(dummy_str);

					face.v_indx.push_back(v_indx);
					face.vt_indx.push_back(vt_indx);
					face.vn_indx.push_back(vn_indx);

					num_edges++;
				}

				switch (num_edges)
				{
				case 3:
					num_triangles++;
					break;
				case 4:
					num_polygons++;
					break;
					/*default:
						std::cerr << "Error in reading the object file " << line << " , " <<  num_edges << std::endl;*/
				}
				face.num_edges = num_edges;
				face.mat_indx = current_mat_indx;
				face_indexes.push_back(face);
			}
			if (iss >> point3)
			{
				if (!dummy_str.compare("v"))
				{
					vs.push_back(point3);
					v_num++;
				}
				else if (!dummy_str.compare("vn"))
				{
					vns.push_back(point3);
					vn_num++;
				}
				else if (!dummy_str.compare("vt"))
				{
					vts.push_back(point3);
					vt_num++;
				}
			}
		}
	}

	if (v_num != file_vs_num)
	std::cout << "Inconsistency in the number of read vs with those in the obj file  " << v_num << "," << file_vs_num << std::endl;
	if (vt_num != file_vts_num)
	std::cout << "Inconsistency in the number of read vts with those in the obj file  " << vt_num << "," << file_vts_num << std::endl;
	if (vn_num != file_vns_num)
	std::cout << "Inconsistency in the number of read vns with those in the obj file  " << vn_num << "," << file_vns_num << std::endl;
	if (num_polygons != file_polygon_num)
	std::cout << "Inconsistency in the number of read polygons with those in the obj file  " << num_polygons << "," << file_polygon_num << std::endl;
	if (num_triangles != file_triangle_num)
	std::cout << "Inconsistency in the number of read triangles with those in the obj file  " << num_triangles << "," << file_triangle_num << std::endl;

	obj_file.close();
}

void obj_model_reader::read_mtl_file()
{
	std::string line;
	std::ifstream mtl_file(mtl_file_name);
	std::stringstream iss;

	double Ns, d, Tr;
	color Ka, Kd, Ks, Tf;
	std::string dummy_str;
	int material_counter = 0;
	while (std::getline(mtl_file, line))
	{
		if (line.length() >= 3)
		{
			iss.clear();
			iss.str("");
			iss << line;
		}
		if (iss >> dummy_str)
		{
			if (dummy_str == "newmtl")
			{
				if (material_counter)
				{
					shared_ptr<material> material_i = make_shared<general>(Kd, Ns, d, Tr, Tf, Ks);
					materials.push_back(material_i);
				}
				iss >> dummy_str;
				std::cout << "Reading the material " << dummy_str << std::endl;
				material_counter++;
			}
			else if (dummy_str == "Ns")
			{
				iss >> Ns;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "d")
			{
				iss >> d;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Tr")
			{
				iss >> Tr;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Tf")
			{
				iss >> Tf;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Ka")
			{
				iss >> Ka;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Kd")
			{
				iss >> Kd;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Ks")
			{
				iss >> Ks;
				iss.clear();
				iss.str("");
			}

		}
	}

	// Since we add to the materials vector whenever we read a newmtl line the last material would not be added otherwise.
	shared_ptr<material> material_i = make_shared<general>(Kd, Ns, d, Tr, Tf, Ks);
	materials.push_back(material_i);
	mtl_file.close();
}

void obj_model_reader::add_item()
{
	int counter = 0;

	for (auto & face : face_indexes)
	{
		std::vector<point3> vs_i;
		std::vector<point3> vts_i;
		std::vector<point3> vns_i;

		counter++;
		std::cout << "Adding the face " << counter << std::endl;
		shared_ptr<material> mat;
		int num_edges = face.num_edges;

		for (int j = 0; j < num_edges; j++)
		{
			if (j >= face.vt_indx.size()) {
				std::cerr << "1-Out of bonds access " << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 < 0 || face.vt_indx[j] - 1 >= this->vs.size()) {
				std::cerr << "2-Out of bonds access" << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 >= this->vts.size()) {
				std::cerr << "3-Out of bonds access"   << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 >= this->vns.size()) {
				std::cout << "4-Out of bonds access" << std::endl;
				std::cout << face.vt_indx[j] << ">=" << this->vns.size() << std::endl;
				continue;
			}

			point3 v_j = this->vs[face.vt_indx[j] - 1]; // 1 based indexing in the obj file
			point3 vt_j = this->vts[face.vt_indx[j] - 1];
			point3 vn_j = this->vns[face.vt_indx[j] - 1];
			vs_i.push_back(v_j);
			vts_i.push_back(vt_j);
			vns_i.push_back(vn_j);
		}

		mat = materials[face.mat_indx];

		switch (num_edges)
		{
		case 3:
			world->add(make_shared<triangle_mesh>(vs, vts, vns, mat));
			break;
		case 4:
			world->add(make_shared<mesh>(vs, vts, vns, mat));
			break;
		}
	}

}



