#pragma once
#include "vector.h"

namespace aline {

class Face {
public:
    uint idP1;
    uint idP2;
    uint idP3;

    Face(uint v1, uint v2, uint v3);
};

}