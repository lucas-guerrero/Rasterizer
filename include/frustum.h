#pragma once
#include "vector.h"
#include "object.h"

#define ALPHA 60
#define PI 3.14159265

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

    real dNear;
    real dFar;

    Frustum(real dNear, real dFar, real a, real e);

    void update(real a, real e);

    Object clipping(const Object &object);

private:
    Object clippingCut(const Object &object, const Vec4r &plan);
    Vec4r interseption(const Vec4r &point, const Vec4r &direction, const Vec4r &plan);
};

}