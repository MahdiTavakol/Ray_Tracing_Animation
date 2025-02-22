#include "camera_gpu_openmp.h"

camera_gpu_openmp::camera_gpu_openmp() :
	camera()
{
	// Grid Dim
	TX = 8;
	TY = 8;

	// Block Dim
	NX = image_width / TX;
	NY = image_height / TY;

}

void camera_gpu_openmp::render(const hittable& world, color_array& c_a)
{
	// Lets test it
	const hittable_list_gpu_openmp* is_it_correct = dynamic_cast<const hittable_list_gpu_openmp*>(&world);
	if (!is_it_correct) std::cerr << "You should never end up here... Please use the appropriate camera!!!" << std::endl;


#pragma omp target teams num_teams(TX*TY) thread_limit(NX*NY)
	{
		int local_thread_id = omp_get_thread_num();
		int team_id = omp_get_team_num();
		int tx = local_thread_id % NX;
		int ty = local_thread_id / NX;
		int team_x = team_id % TX;
		int team_y = team_id / TX;
		int i = tx + team_x * NX;
		int j = ty + team_y * NY;

		if (i > image_width || j > image_height) return;


		color pixel_color(0, 0, 0);
		for (int sample = 0; sample < samples_per_pixel; sample++)
		{
			ray r = get_ray(i, j);
			pixel_color += ray_color(r, max_depth, world);
		}
		pixel_color = pixel_samples_scale * pixel_color;
		color_data** c_data = c_a.return_array();
		c_data[i][j].r = pixel_color.x();
		c_data[i][j].g = pixel_color.y();
		c_data[i][j].b = pixel_color.z();
	}
}

#pragma omp declare target
color camera_gpu_openmp::ray_color(const ray& _r, int depth, const hittable& world) const
{
	// Lets test it
	const hittable_list_gpu_openmp* world_gpu_openmp = dynamic_cast<const hittable_list_gpu_openmp*>(&world);
	if (!world_gpu_openmp) std::cerr << "You should have never ended up here... Please use the appropriate camera!!!" << std::endl;


	// Since GPUs have limited support for recursion, the recursion is converted to a loop
	hit_record rec;
	color product, sum_product;

	ray scattered;
	color attenuation;
	color color_from_emission;
	color output_color;

	int temp_mat_info[2] = { 0,0 };

	if (!world_gpu_openmp->hit(_r, interval(0.001, infinity),  temp_mat_info))
		return background;
	


	color* color_from_emission_array = new color[depth];
	color* attenuation_array = new color[depth];

	ray scattered;

	int material_type = temp_mat_info[0];
	int material_id = temp_mat_info[1];

	// I know it is a branching, but we have just three branches. it is better than having branching over over million rays

	for (int i = 0; i < depth; i++) {
		if (material_type == 0) {
			const metal mat = world_gpu_openmp->metals[material_id];
			color_from_emission = mat.emitted(rec.u, rec.v, rec.p);
			if (mat.scatter(_r, rec, attenuation, scattered))
				attenuation = color(0, 0, 0);
		}
		else if (material_type == 1) {
			const lambertian mat = world_gpu_openmp->lambertians[material_id];
			color_from_emission = mat.emitted(rec.u, rec.v, rec.p);
			if (mat.scatter(_r, rec, attenuation, scattered))
				attenuation = color(0, 0, 0);
		}
		else if (material_type == 2) {
			const dielectric mat = world_gpu_openmp->dielectrics[material_id];
			color_from_emission = mat.emitted(rec.u, rec.v, rec.p);
			if (mat.scatter(_r, rec, attenuation, scattered))
				attenuation = color(0, 0, 0);
		}
		attenuation_array[i] = attenuation;
		color_from_emission_array[i] = color_from_emission;
	}
		
	for (int i = 0; i < depth; i++)
		output_color = output_color * attenuation_array[depth - 1 - i] + color_from_emission_array[depth - 1 - i];


	delete[] color_from_emission_array;
	delete[] attenuation_array;

}
#pragma omp end declare target
