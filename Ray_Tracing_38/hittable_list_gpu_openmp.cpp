#include "hittable_list_gpu_openmp.h"

hittable_list_gpu_openmp::hittable_list_gpu_openmp():
	nshapes(0), nspheres(0), nquads(0), ntriangles(0),
	ncircles(0), nmaterials(0), nmetals(0), nlambertians(0),
	ndielectrics(0), shapes_mat_info(nullptr),
	sphere_ids(nullptr), quad_ids(nullptr), triangle_ids(nullptr),
	circle_ids(nullptr),spheres(nullptr),quads(nullptr),circles(nullptr),
	metals(nullptr),lambertians(nullptr),dielectrics(nullptr)
{}

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
	while(nspheres)
		spheres[--nspheres].~sphere();

	while (nquads)
		quads[--nquads].~quad();

	while (ncircles)
		circles[--ncircles].~circle();

	free(spheres);
	free(quads);
	free(circles);

	while (nmetals)
		metals[--nmetals].~metal();
	while (nlambertians)
		lambertians[--nlambertians].~lambertian();
	while (ndielectrics)
		dielectrics[--ndielectrics].~dielectric();

#pragma omp target exit data map(delete: shape_mat_info[0:nshapes][0:2])
#pragma omp target exit data map(delete: spheres[0:nspheres])
#pragma omp target exit data map(delete: sphere_ids[0:nspheres])
#pragma omp target exit data map(delete: quads[0:nquads])
#pragma omp target exit data map(delete: quad_ids[0:nquads])
#pragma omp target exit data map(delete: circles[0:ncircles])
#pragma omp target exit data map(delete: circle_ids[0:ncircles])

#pragma omp target exit data map(delete: metals[0:nmetals])
#pragma omp target exit data map(delete: lambertians[0:nlambertians])
#pragma omp target exit data map(delete: dielectrics[0:ndielectrics])

}

void hittable_list_gpu_openmp::initialize()
{
	std::vector<int[2]> shapes_mat_info_temp;
	std::vector<sphere> spheres_temp;
	std::vector<int> sphere_ids_temp;
	std::vector<quad> quads_temp;
	std::vector<int> quad_ids_temp;
	std::vector<circle> circles_temp;
	std::vector<int> circle_ids_temp;
	/*
	 *  Other shapes
	 */
	std::vector<metal> metals_temp;
	std::vector<lambertian> lambertians_temp;
	std::vector<dielectric> dielectrics_temp;


	int metal_id = 0;
	int lambertian_id = 0;
	int dielectric_id = 0;
	// Other shapes


	int shape_id = 0;
	for (std::shared_ptr<hittable> object : objects)
	{
		hittable* object_raw = object.get();

		sphere* is_sphere = dynamic_cast<sphere*>(object_raw);
		quad* is_quad = dynamic_cast<quad*>(object_raw);
		circle* is_circle = dynamic_cast<circle*>(object_raw);


		/*
		 *  Other shapes
		 */

		if (is_sphere) {
			spheres_temp.push_back(*is_sphere);
			sphere_ids_temp.push_back(shape_id++);
		} else if (is_quad) {
			quads_temp.push_back(*is_quad);
			quad_ids_temp.push_back(shape_id++);
		} else if (is_circle) {
			circles_temp.push_back(*is_circle);
			circle_ids_temp.push_back(shape_id++);
		} else {
			std::cout << "Warning: Unsupported Shape: Skipping it!" << std::endl;
			continue;
		}

		std::shared_ptr<material> temp_mat;
		object->return_material(temp_mat);
		material* material_raw = temp_mat.get();

		metal* is_metal = dynamic_cast<metal*>(material_raw);
		lambertian* is_lambertian = dynamic_cast<lambertian*>(material_raw);
		dielectric* is_dielectric = dynamic_cast<dielectric*>(material_raw);

		int mat_info_shape_i[2] = { 0,0 };
		if (is_metal) {
			mat_info_shape_i[0] = 0;
			mat_info_shape_i[1] = metal_id++;
			metals_temp.push_back(*is_metal);
		} else if (is_lambertian) {
			mat_info_shape_i[0] = 1;
			mat_info_shape_i[1] = lambertian_id++;
			lambertians_temp.push_back(*is_lambertian);
		} else if (is_dielectric) {
			mat_info_shape_i[0] = 2;
			mat_info_shape_i[1] = dielectric_id++;
			dielectrics_temp.push_back(*is_dielectric);
		} else {
			std::cout << "Warning: Unsupported Material: Adding Generic Metal" << std::endl;
			metal temp_metal = metal();
			mat_info_shape_i[0] = 0;
			mat_info_shape_i[1] = metal_id++;
			metals_temp.push_back(temp_metal);
		}
		shapes_mat_info_temp.push_back(mat_info_shape_i);
	}

	nspheres = spheres_temp.size();
	nquads = quads_temp.size();
	ncircles = circles_temp.size();

	/*
	 *  Other shapes
	 */
	nshapes = nspheres + nquads + ncircles;

	int* temp = (int*)malloc(nshapes * 2 * sizeof(int));
	shapes_mat_info = (int**)malloc(nshapes * sizeof(int*));
	for (int i = 0; i < nshapes; i++)
		shapes_mat_info[i] = &temp[2 * i];



	spheres = (sphere*)malloc(nspheres * sizeof(sphere));
	quads = (quad*)malloc(nquads * sizeof(quad));
	circles = (circle*)malloc(ncircles * sizeof(circle));
	sphere_ids = (int*)malloc(nspheres * sizeof(int));
	quad_ids = (int*)malloc(nquads * sizeof(int));
	circle_ids = (int*)malloc(ncircles * sizeof(int));

	std::copy(sphere_ids_temp.begin(), sphere_ids_temp.end(), sphere_ids);
	std::copy(quad_ids_temp.begin(), quad_ids_temp.end(), quad_ids);
	std::copy(circle_ids_temp.begin(), circle_ids_temp.end(), circle_ids);

	/*
	 *  Other shapes
	 */

	for (int i = 0; i < nshapes; i++)
	{
		shapes_mat_info[i][0] = shapes_mat_info_temp[i][0];
		shapes_mat_info[i][1] = shapes_mat_info_temp[i][1];
	}

	for (int i = 0; i < nspheres; i++)
	{
		new (&spheres[i]) sphere(spheres_temp[i]);
	}

	for (int i = 0; i < nquads; i++)
	{
		new (&quads[i]) quad(quads_temp[i]);
	}

	for (int i = 0; i < ncircles; i++)
	{
		new (&circles[i]) circle(circles_temp[i]);
	}

	/*
	 *  Other shapes
	 */

	nmetals = metals_temp.size();
	nlambertians = lambertians_temp.size();
	ndielectrics = dielectrics_temp.size();

	metals = (metal*)malloc(nmetals * sizeof(metal));
	lambertians = (lambertian*)malloc(nlambertians * sizeof(lambertian));
	dielectrics = (dielectric*)malloc(ndielectrics * sizeof(dielectric));

	for (int i = 0; i < nmetals; i++)
		new (&metals[i]) metal(metals_temp[i]);
	for (int i = 0; i < nlambertians; i++)
		new (&lambertians[i]) lambertian(lambertians_temp[i]);
	for (int i = 0; i < ndielectrics; i++)
		new (&dielectrics[i]) dielectric(dielectrics_temp[i]);

// Copying the shapes information to the GPU
#pragma omp target enter data map(to: shape_mat_info[0:nshapes][0:2])
#pragma omp target enter data map(to: sphere_ids[0:nspheres])
#pragma omp target enter data map(to: spheres[0:nspheres])
#pragma omp target enter data map(to: quad_ids[0:nquads])
#pragma omp target enter data map(to: quads[0:nquads])
#pragma omp target enter data map(to: circle_ids[0:ncircles])
#pragma omp target enter data map(to: circles[0:ncircles])
/*
 *  Other shapes
 */

// Copying the materials info to the GPU
#pragma omp target enter data map(to: metals[0:nmetals])
#pragma omp target enter data map(to: lambertians[0:nlambertians])
#pragma omp target enter data map(to: dielectric[0:ndielectric])

/*
 * Other materials
 */


}

bool hittable_list_gpu_openmp::hit(const ray& r, interval ray_r, hit_record& rec) const
{
	std::cerr << "This is the gpu_openmp version of the hit so we should not have reached here" << std::endl;
}

bool hittable_list_gpu_openmp::hit(const ray& r, interval ray_t, int _mat_info[2]) const
{
	hit_record_gpu_openmp temp_rec;
	int temp_mat_info[2];
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	for (int i = 0; i < nspheres; i++)
	{
		if (spheres[i].hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			int id = sphere_ids[i];
			std::copy(shapes_mat_info[id], shapes_mat_info[id]+2, temp_mat_info);
		}
	}

	for (int i = 0; i < nquads; i++)
	{
		if (quads[i].hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			int id = quad_ids[i];
			std::copy(shapes_mat_info[id], shapes_mat_info[id] + 2, temp_mat_info);
		}
	}

	for (int i = 0; i < ncircles; i++)
	{
		if (circles[i].hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			int id = circle_ids[i];
			std::copy(shapes_mat_info[id], shapes_mat_info[id] + 2, temp_mat_info);
		}
	}

	return hit_anything;
}
