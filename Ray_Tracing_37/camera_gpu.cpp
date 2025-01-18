#include "camera_gpu.h"

camera_gpu::camera_gpu(hittable_list* _world_h)
	: world_h(_world_h)
{
	// color_data
	allocate_memory();

	// world
	initialize_device_memory();
	fill_world_d();

	// camera
	initialize_streams();
	initialize_camera<<<1,1>>>();
}

camera_gpu::~camera_gpu()
{
	for (int i = 0; i < num_streams; i++)
		hipStreamDestroy(streams[i]);

	hipHostFree(c_data_h);
	hipFree(c_data_d);
}



void camera_gpu::render(color_array& c_a)
{
	for (int i = 0; i < num_streams; i++)
	{
		render_stream(i);
		copy_device_to_host(i);
	}

	// Synchronize stream
	for (int i = 0; i < num_streams; i++)
		hipStreamSynchronize(streams[i]);

	gather_streams(c_a);
}

void camera_gpu::copy_device_to_host(const int _stream_number)
{
	hipStream stream = streams[_stream_number];
	int hittable_size = sizeof(hittable);
	
	int data_size = data_num_stream[_stream_number] * sizeof(color_data);

	int offset = min_stream_width[_stream_number] + image_width * min_stream_height[_stream_number];

	/*
	*  We setup the world on the device and then asks it for the color_data_matrix
	*  So for the DEVICE_TO_HOST we just need to transfer the color_data_matrix
	*  and in the opposite direction there is a need for just world_d
	*/
	
	HIP_CHECK(hipMemcpyAsync(c_data_h+offset, c_data_d+offset, data_size, hipMemcpyDeviceToHost, stream));
}

void camera_gpu::gather_streams(color_array& c_a)
{
	color_data** c_data = c_a.return_array();

	for (int i = 0; i < image_width; i++)
		for (int j = 0; j < image_height; j++)
		{
			int index = i + j * image_width;
			c_data[i][j] = c_data_h[index];
		}
}

void camera_gpu::allocate_memory()
{
	hipMallocHost((void **)&c_data_h, image_width * image_height * sizeof(color_data));
	hipMalloc((void **)&c_data_d, image_width * image_height * sizeof(color_data));
}

void camera_gpu::initialize_device_memory()
{
	double** sphere_centers_h, * sphere_radii_h;
	double** sphere_centers_d, * sphere_radii_d;
	int* sphere_mat_type_d, * sphere_mat_type_h;
	double** albedo_h, * fuzz_h;
	double** albedo_d, * fuzz_d;
	int n_spheres = 0;
	int n_materials = 0;

	std::unordered_set<std::shared_ptr<material>> unique_materials;

	// Counting the number of spheres and unique materials
	for (auto& object: world_h->objects)
	{
		if (std::shared_ptr<sphere> sphere_i =  std::dynamic_pointer_cast<sphere>(object))
		{
			ray _ray;
			double _radius;
			std::shared_ptr<material> _mat;
			sphere_i->return_params(_ray, _radius, _mat);
			n_spheres++;

			unique_materials.insert(_mat);
		}
		else
		{
			std::cout << "Currently only sphere types is supported for the GPU!" << std::endl;
		}
	}

	n_materials = unique_materials.size();

	hipMallocHost((void**)&sphere_centers_h, n_spheres * 6 * sizeof(double));
	hipMallocHost((void**)&sphere_radii_h, n_spheres * sizeof(double));
	hipMallocHost((void**)&sphere_mat_type_h, n_spheres * sizeof(int));
	hipMallocHost((void**)&albedo_h,3* n_materials * sizeof(double));
	hipMallocHost((void**)&fuzz_h, n_materials * sizeof(double));
	hipMalloc((void**)&sphere_centers_d, n_spheres * 6 * sizeof(double));
	hipMalloc((void**)&sphere_radii_d, n_spheres * sizeof(double));
	hipMalloc((void**)&sphere_mat_type_d, n_spheres * sizeof(int));
	hipMalloc((void**)&albedo_d,3* n_materials * sizeof(double));
	hipMalloc((void**)&fuzz_d, n_materials * sizeof(double));

	for (auto it = unique_materials.begin(); it != unique_materials.end(); ++it) {
		color albedo;
		double fuzz;
		std::shared_ptr<material> test = *it;
		metal* s = dynamic_cast<metal>(test);
		test->return_params(albedo, fuzz);
	}

	for (int i = 0; i < n_materials; i++)
	{
		color albedo;
		double fuzz;
		unique_materials[i]->return_params(albedo, fuzz);
	}


	int counter = 0;
	for (auto& object : world_h->objects)
	{
		if (std::shared_ptr<sphere> sphere_i = std::dynamic_pointer_cast<sphere>(object)) {
			ray _ray;
			double _radius;
			point3 _center;
			vec3 _dir;
			std::shared_ptr<material> _mat;
			sphere_i->return_params(_ray, _radius, _mat);
			_center = _ray.origin();
			_dir = _ray.direction();
			sphere_centers_h[counter][0] = _center.x();
			sphere_centers_h[counter][1] = _center.y();
			sphere_centers_h[counter][2] = _center.z();
			sphere_centers_h[counter][3] = _dir.x();
			sphere_centers_h[counter][4] = _dir.y();
			sphere_centers_h[counter][5] = _dir.z();
			sphere_radii_h[counter] = _radius;
			sphere_mat_type_h[counter] = 0;
			counter++;
		}
	}

	HIP_CHECK(hipMemcpy(sphere_centers_d, sphere_centers_h, 6 * n_spheres * sizeof(double), hipMemcpyHostToDevice));
	HIP_CHECK(hipMemcpy(sphere_radii_d, sphere_radii_h, n_spheres * sizeof(double), hipMemcpyHostToDevice));
	HIP_CHECK(hipMemcpy(sphere_mat_type_d, sphere_mat_type_h, n_spheres * sizeof(int), hipMemcpyHostToDevice));

}

void camera_gpu::fill_world_d()
{
	create_spheres_on_device<<<1,1>>>();
}

void camera_gpu::initialize_streams()
{
	for (int i = 0; i < num_streams; i++)
		hipStreamCreate(&streams[i]);
}



