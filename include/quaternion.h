#pragma once
#include "vector.h"
#include "matrix.h"

namespace aline {

class Quaternion {
public:
    Vec3r v;
    real s;

    Quaternion(real s, const Vec3r v);

    Mat44r transformToMatrix();

    Quaternion mult(const Quaternion &q);
};

const Quaternion operator*(const Quaternion &q1, const Quaternion &q2) ;

}