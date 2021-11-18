#pragma once
#include "vector.h"

namespace aline {

class Vertex {
public:
    Vec2r point;
    real intensity;

    Vertex(const Vec2r &c, real h);
};

}