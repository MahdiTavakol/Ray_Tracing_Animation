#ifndef COLOR_ARRAY_H
#define COLOR_ARRAY_H

#include "rtweekend.h"



#include <stdlib.h>
#include <iostream>
#include "interval.h"

typedef struct { double r, g, b; } color_data;

class color_array
{
public:
    color_array(const int& _width, const int& _height, color_data** _color_data);
    color_array(const int& _width, const int& _height, color_data* _color_data);
    color_array(const int& _width, const int& _height);
    color_array(const color_array& );
    color_array();
    ~color_array();

    void reset_size(const int& _width, const int& _height);
    void reset(const int& _width, const int& _height, color_data* _color_data);
    void set_range(const int& _width, const int& _height);
    color_array& operator=(color_array second);
    color_data**& return_array();

private:
    int width, height;
    color_data** array;

    void allocate();
    void deallocate();
};

inline std::ostream& operator<<(std::ostream& out, const color_data& _c_data)
{
    static const interval intensity(0.000, 0.999);
    double r = _c_data.r;
    double g = _c_data.g;
    double b = _c_data.b;
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
    return out << rbyte << " " << gbyte << " " << bbyte << std::endl;
}

#endif
