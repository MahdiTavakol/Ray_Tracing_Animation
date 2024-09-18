#ifndef POINT3_ANIMATED_H
#define POINT3_ANIMATED_H

#include "rtweekend.h"

#include "vec3.h"

class point3_animated
{
    friend point3_animated& interpolate(const point3_animated& first, const point3_animated& second, const double& _time);
public:
    point3_animated() : time(0) { loc = point3(); }
    point3_animated(double _time) : time(_time) { loc = point3(); }
    point3_animated(double& _x, double& _y, double& _z, double& _time) : time(_time) {loc = point3(_x,_y,_z); }
	point3_animated(point3 _loc, double _time) : time(_time), loc(_loc) {}
	point3_animated(const point3_animated& _point): time(_point.time), loc(_point.loc) {}
	
    const point3& return_loc() const { return loc; }
	const double& return_time() const { return time; }
    const double& x() const { return loc.x(); }
    const double& y() const { return loc.y(); }
    const double& z() const { return loc.z(); }



protected:
	double time;
	point3 loc;
};




inline static point3_animated& interpolate(const point3_animated& first, const point3_animated& second, const double& _time)
{
    point3_animated output(_time);

    if (abs(_time - first.time) < 0.001)
    {
        output = first;
        return output;
    }
    if (abs(_time - second.time) < 0.001)
    {
        output = second;
        return output;
    }


    for (int i = 0; i < 3; i++)
    {
        output.loc[i] = first.loc[i] + (_time - first.time) * (second.loc[i] - first.loc[i]) / (second.time - first.time);
    }
    return output;
}

#endif
