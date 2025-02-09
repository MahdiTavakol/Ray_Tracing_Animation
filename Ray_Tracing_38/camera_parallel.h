#ifndef CAMERA_PARALLEL_H
#define CAMERA_PARALLEL_H

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "color_array.h"
#include "hittable.h"
#include "input.h"
#include "vec3.h"

class parallel;

class camera_parallel : public camera
{
public:
    friend class parallel;
    camera_parallel(const input* _in);
    camera_parallel(const int _width_min, const int _width_max, const int _height_min, const int _height_max);
    camera_parallel();
    void render(const hittable& world, color_array& c_a);
    void set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max) override;

private:
    int height_min, height_max, width_min, width_max;
};

#endif
