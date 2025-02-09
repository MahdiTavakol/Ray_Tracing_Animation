#include "hittable_list_gpu_openmp.h"

hittable_list_gpu_openmp::hittable_list_gpu_openmp():
	nlocal(0), nmax(100)
{
	objects = new hittable * [nmax];
#pragma omp target enter data map(alloc: objects[:nmax])
}

hittable_list_gpu_openmp::hittable_list_gpu_openmp(shared_ptr<hittable> _object)
{
	add(_object);
}

hittable_list_gpu_openmp::~hittable_list_gpu_openmp()
{
	clear();
}

void hittable_list_gpu_openmp::clear()
{
	nmax = 0;

	while (nlocal)
	{
		objects[nlocal - 1]->~hittable();
		nlocal--;
	}

#pragma omp target exit data map(delete(objects[:nmax]))
	delete[] objects;
	objects = nullptr;
}

void hittable_list_gpu_openmp::add(shared_ptr<hittable> object)
{
	if (nlocal >= nmax)
	{
		nmax *= 2;
		hittable** objects_new = new hittable * [nmax];
#pragma omp target enter data map(alloc: objects_new[:nmax])

#pragma omp target 
		for (int i = 0; i < nlocal; i++)
			objects_new[i] = objects[i];

		delete[] objects;
		objects = objects_new;
	}


	hittable* object_raw = object.get();

	if (sphere* shape = dynamic_cast<sphere*>(object_raw)) {
		sphere* shape_gpu;

#pragma omp target enter data map(alloc:shape_gpu)
#pragma omp target enter data map(to: *shape, )
		{
			shape_gpu = new sphere(*shape);
		}
#pragma omp target 
		objects[nlocal++] = shape_gpu;
	}
	else if (quad* shape = dynamic_cast<quad*>(object_raw)) {
		quad* shape_gpu;
#pragma omp target enter data map(alloc:shape_gpu)
#pragma omp target enter data map(to:*shape)
		{
			shape_gpu = new quad(*shape);
		}
#pragma omp target 
		objects[nlocal++] = shape_gpu;
	}
}

bool hittable_list_gpu_openmp::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	for (const auto& object : objects)
	{
		if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}
