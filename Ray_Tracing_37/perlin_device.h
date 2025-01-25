#ifndef PERLIN_DEVICE_H
#define PERLIN_DEVICE_H

#include "color_device.h"

class perlin_device
{
public:
	perlin_device();
	double noise(const point3_device& p) const;
	double turb(const point3_device& _p, int _depth) const;
private:
	static const int point_count = 256;
	vec3_device randvec[point_count];
	int perm_x[point_count];
	int perm_y[point_count];
	int perm_z[point_count];

	static double trilinear_interp(double c[2][2][2], double u, double v, double w);
	static double perlin_interp(const vec3_device c[2][2][2], double u, double v, double w);
	static void perlin_generate_perm(int* p);
	static void permute(int* p, int n);
};

#endif