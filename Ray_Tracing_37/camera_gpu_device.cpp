#include "camera_gpu.h"

__global__ void camera_gpu::initialize_camera()
{
	image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	pixel_samples_scale = 1.0 / samples_per_pixel;

	center = lookfrom;

	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta / 2.0);
	auto viewport_height = 2 * h * focus_dist;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);


	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}

__global__ void camera_gpu::create_spheres_on_device(int _n_spheres, double** _sphere_centers_d, double* _sphere_radii_d, int* _sphere_mat_type_d, int* _sphere_mat_id_d)
{
	sphere_device* spheres;
	HIP_CHECK(hipMalloc((void**)&spheres, _n_spheres * sizeof(sphere_device)));


	// Creating each sphere on the device
	for (int i = 0; i < _n_spheres; i++)
	{
		point3_device center(_sphere_centers_d[i][0], _sphere_centers_d[i][1], _sphere_centers_d[i][2]);
		vec3_device dir(_sphere_centers_d[i][3], _sphere_centers_d[i][4], _sphere_centers_d[i][5]);
		ray_device ry(center, dir);

		/* with copy constructor
		 * sphere_device sphere_i(ry, _sphere_radii_d[i]);
		 * spheres[i] = sphere_i;
		 */

		// without copy constructor
		new(&spheres[i]) sphere_device(ry, _sphere_radii_d[i]);
	}

	new(world_d) hittable_list_device(spheres, _sphere_mat_type_d, _sphere_mat_id_d, _n_spheres);
}

__global__ void camera_gpu::create_metals_on_device(int _n_metals, double** _albedo_d, double* _fuzz_d)
{
	new(material_d) material_list_device(_n_metals, _albedo_d, _fuzz_d);

}

__global__ void camera_gpu::destroy_world_d()
{
	world_d->~hittable_list_device();
	HIP_CHECK(hipFree(world_d));
}

__global__ void camera_gpu::destroy_material_d()
{
	material_d->~material_list_device();
	HIP_CHECK(hipFree(material_d));
}

__global__ void camera_gpu::render_stream(int _stream_number)
{

	int imin = min_stream_width[_stream_number];
	int imax = max_stream_width[_stream_number];
	int jmin = min_stream_height[_stream_number];
	int jmax = max_stream_height[_stream_number];


	int idx = threadIdx.x + blockDim.x * blockIdx.x;
	int idy = threadIdx.y + blockDim.y * blockIdx.y;

	int i = idx + imin;
	int j = idy + jmin;

	if (i > imax || j > jmax)
		return;


	color_device pixel_color(0, 0, 0);
	
	for (int sample = 0; sample < samples_per_pixel; sample++)
	{
		ray_device r = get_ray(i, j);
		pixel_color += ray_color(r, max_depth, world_d);
	}

	pixel_color = pixel_samples_scale * pixel_color;

	int index = i + j * image_width;

	c_data_d[index].r = pixel_color.x();
	c_data_d[index].g = pixel_color.y();
	c_data_d[index].b = pixel_color.z();
}

__device__ ray_device camera_gpu::get_ray(int i, int j) const
{
	auto offset = sample_square();
	auto pixel_sample = pixel00_loc
		+ ((i + offset.x()) * pixel_delta_u)
		+ ((j + offset.y()) * pixel_delta_v);

	auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;
	auto ray_time = random_double();

	return ray_device(ray_origin, ray_direction, ray_time);
}

__device__ vec3_device camera_gpu::sample_square() const
{
	return vec3_device(random_double() - 0.5, random_double() - 0.5, 0);
}

__device__ point3_device camera_gpu::defocus_disk_sample() const
{
	auto p = random_in_unit_disk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

__device__ color_device camera_gpu::ray_color(const ray_device& _r, int depth, const hittable_list_device* world_d) const
{
	// Since GPUs have limited support for recursion, the recursion is converted to a loop
	hit_record_device rec;
	color product, sum_product;

	ray_device scattered;
	color_device attenuation;
	color_device color_from_emission;
	color_device output_color;
	

	if (!world_d->hit(_r,interval_device(0.001,infinity), rec))
		return background;


	color_device* color_from_emission_array = new color_device[depth];
	color_device* attenuation_array = new color_device[depth];
	
	ray_device scattered;



	for (int i = 0; i < depth; i++) {

		if (rec.mat_type != 0) std::cerr << "Unsupported material" << std::endl;

		const metal_device&  mat = (*material_d)(rec.mat_type, rec.mat_id);
		
		color_from_emission = mat.emitted(rec.u, rec.v, rec.p);

		if (mat.scatter(_r,rec,attenuation,scattered))
			attenuation = color(0,0,0);

		attenuation_array[i] = attenuation;
		color_from_emission_array[i] = color_from_emission;
	}

	output_color = color_device(0,0,0);
	
	for (int i = 0; i < depth; i++)
		output_color = output_color * attenuation_array[depth - 1 - i] + color_from_emission_array[depth - 1 - i];


	delete[] color_from_emission_array;
	delete[] attenuation_array;

}


