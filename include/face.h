#pragma once
#include "vector.h"
#include "color.h"

namespace aline {

class Face {
public:
    uint idP1;
    uint idP2;
    uint idP3;
    minwin::Color color;

    Face(uint v1, uint v2, uint v3, const minwin::Color &color);
};

}