#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>
#include "point3_animated.h"

class path
{
public:
	path(std::string _filename, int _num_seconds, int _fps);
	path(point3& center, double& radius, int& _num_seconds, int& _fps, double& theta);
	path(point3& _point); // For backward compatibility with the static case.
	~path() {}

	const point3_animated& operator[](int i) const { return locs[i]; }


private:
	int fps,num_frames, num_seconds, num_init_frames;
	std::string filename;
	std::vector<point3_animated> init_locs;
	std::vector<point3_animated> locs;
	void read_file();
	void interpolate_points();
};

#endif
