#include <iostream>
#include <fstream>

#include <mpi.h>


#include "rtweekend.h"
#include "Renderer.h"

void message(int _rank, std::string _text);

int main(int argc, char** argv)
{
	// MPI
	MPI_Init(&argc, &argv);

	int mode, rank, num_frames;
	std::string filename;
	//mode = RANDOM_SPHERES;
	//mode = CHECKER_BOARDS;
	//mode = RANDOM_SPHERES;
	//mode = EARTH_SPHERE;
	//mode = PERLIN_SPHERE;
	//mode = QUADS;
	//mode = SIMPLE_LIGHT;
	//mode = TWO_LIGHTS;
	//mode = CORNELL_BOX;
	//mode = TWO_BOXES;
	//mode = TWO_BOXES_ROTATED;
	//mode = CORNELL_SMOKE;
	//mode = OBJ_MODEL;
	//mode = OBJ_MODEL_PARALLEL;
	//mode = RANDOM_SPHERES_ANIMATED;
	mode = RANDOM_SPHERES_GPU_OPENMP;

	//filename = "RT18.ppm";
	//filename = "RT19.ppm";
	//filename = "RT20.ppm";
	//filename = "RT21.ppm";
	//filename = "RT22.ppm";
	//filename = "RT23.ppm";
	//filename = "RT24.ppm";
	//filename = "RT25.ppm";
	//filename = "RT26.ppm";
	//filename = "RT27.ppm";
	//filename = "RT28.ppm";
	//filename = "RT29.ppm";
	//filename = "RT30.ppm";
	//filename = "RT31.ppm";
	//filename = "RT32.ppm";
	filename = "RT33.ppm";

	renderer* render = new renderer(argc, argv, mode, filename);

	rank = render->para_ptr()->return_rank();


	render->setup();

	num_frames = render->return_num_frames();


	for (int i = 0; i < num_frames; i++)
	{
		message(rank, "Moving the camera to the frame " + std::to_string(i));

		render->move_camera(i);

		message(rank, "Rendering the frame " + std::to_string(i) + "                ");

		render->render();

		message(rank, "Gathering data from nodes for frame " + std::to_string(i));

		render->gather();

		message(rank, "Writing the data for frame " + std::to_string(i) + "     ");

		// change the output file name
		render->write_file();

		if (i < num_frames - 1)
		{
			message(rank, "Closing the current file and opening the next one");
			// closing the current file and opening the next one
			render->next_file(i);
		}
	}

	
	MPI_Finalize();
}

void message(int _rank, std::string _text)
{
	if (_rank == 0)
	{
		std::clog << _text << std::endl;
		fflush(stdout);
	}
}