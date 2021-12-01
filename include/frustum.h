#pragma once
#include "vector.h"

#define ALPHA 60
#define PI 3.14159265
#define F 100
#define N 2

namespace aline {

class Frustum {
public:
    // Normal des plan du frustum normaliser
    Vec4r near;
    Vec4r far;
    Vec4r left;
    Vec4r right;
    Vec4r bottom;
    Vec4r top;

    real alpha;

    Frustum(real a);
};

}