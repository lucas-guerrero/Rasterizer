#include "quaternion.h"

using namespace aline;

Quaternion::Quaternion(real s, const Vec3r v): v(v), s(s) {}

Mat44r Quaternion::transformToMatrix() {
    real w = s;
    real x = v[0];
    real y = v[1];
    real z = v[2];

    real x2 = x*x;
    real y2 = y*y;
    real z2 = z*z;

    return Mat44r {
        {1 - 2*y2 - 2*z2, 2*x*y - 2*w*z, 2*x*z + 2*w*y, 0},
        {2*x*y + 2*w*z, 1 - 2*x2 - 2*z2, 2*y*z - 2*w*x, 0},
        {2*x*z - 2*w*y, 2*y*z + 2*w*x, 1 - 2*x2 - 2*y2, 0},
        {0, 0, 0, 1}
    };
}

Quaternion Quaternion::mult(const Quaternion &q){
    Vec3r v2 = q.v;

    real s4 = s * q.s - dot(v, v2);

    Vec3r v4 = s * v2;
    v4 += q.s * v;
    v4 += v * v2;

    return Quaternion(s4, v4);
}

const Quaternion operator*(const Quaternion &q1, const Quaternion &q2){
    Vec3r v1 = q1.v;
    Vec3r v2 = q2.v;

    real s = q1.s * q2.s - dot(v1, v2);

    Vec3r v = q1.s * v2;
    v += q2.s * v1;
    v += v1 * v2;

    return Quaternion(s, v);
}