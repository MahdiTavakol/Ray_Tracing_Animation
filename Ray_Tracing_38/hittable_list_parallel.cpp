#include "hittable_list_parallel.h"


bool hittable_list_parallel::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	std::cout << "Point A1" << std::endl;
	hit_record temp_rec;
	hit_record_struct temp_rec_struct;
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int hit_anything = 0;
	int hit_anything_all = 0;
	auto closest_so_far = ray_t.max;


	for (const auto& object : objects)
	{
		if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = rank;
			closest_so_far = temp_rec.t;
			// fill the temp_rec_struct with temp_rec and then broadcast it.
			// Then after the broadcast I would know which proc has hit and then 
			// put its material into the rec.
			hit_anything = rank;
			temp_rec_struct = hit_record_to_struct(temp_rec);
		}
	}
	MPI_Allreduce(&hit_anything, &hit_anything_all, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	if (hit_anything_all > size)
		std::cerr << "A ray hits objects in two different MPI ranks " << std::endl;
	int count = sizeof(hit_record_struct) / sizeof(double);
	MPI_Bcast(&temp_rec_struct, count, MPI_DOUBLE, hit_anything_all, MPI_COMM_WORLD);

	return hit_anything_all;
}

hit_record_struct hit_record_to_struct(const hit_record& hit_rec)
{
	hit_record_struct hit_rec_struct;
	hit_rec_struct.p = hit_rec.p;
	hit_rec_struct.normal = hit_rec.normal;
	hit_rec_struct.t = hit_rec.t;
	hit_rec_struct.u = hit_rec.u;
	hit_rec_struct.w = hit_rec.w;
	hit_rec_struct.front_face = hit_rec.front_face;
	return hit_rec_struct;
}
