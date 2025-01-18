#ifndef INTERVAL_DEVICE_H
#define INTERVAL_DEVICE_H

#include "rtweekend.h"
#include "rtweekend_gpu.h"

class interval_device {
public:
	double min, max;

	__device__ interval_device() : min(+infinity), max(-infinity) {}

	__device__ interval_device(double _min, double _max) : min(_min), max(_max) {}

	__device__ interval_device(const interval_device& a, const interval_device& b)
	{
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	__device__ double size() const
	{
		return max - min;
	}

	__device__ bool contains(double x) const {
		return min <= x && x <= max;
	}

	__device__ bool surrounds(double x) const
	{
		return min < x && x < max;
	}

	__device__ double clamp(double x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	__device__ interval_device expand(double delta) const {
		auto padding = delta / 2;
		return interval_device(min - padding, max + padding);
	}

	__device__ interval_device operator+(double _displacement)
	{
		return interval_device(this->min + _displacement, this->max + _displacement);
	}

	__device__ static const interval_device empty, universe;
};




#endif