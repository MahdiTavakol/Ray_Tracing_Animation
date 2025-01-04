#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class interval {
public:
	double min, max;

	interval() : min(+infinity), max(-infinity) {}

	interval(double _min, double _max) : min(_min), max(_max) {}

	interval(const interval& a, const interval& b)
	{
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	double size() const
	{
		return max - min;
	}

	bool contains(double x) const {
		return min <= x && x <= max;
	}

	bool surrounds(double x) const
	{
		return min < x && x < max;
	}

	double clamp(double x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	interval expand(double delta) const {
		auto padding = delta / 2;
		return interval(min - padding, max + padding);
	}

	interval operator+(double _displacement)
	{
		return interval(this->min + _displacement, this->max + _displacement);
	}

	static const interval empty, universe;
};




#endif