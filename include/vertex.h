#pragma once
#include "vector.h"

namespace aline {

class Vertex {
public:
    Vec3r point;
    real intensity;

    Vertex(const Vec3r &c, real h);

};

}