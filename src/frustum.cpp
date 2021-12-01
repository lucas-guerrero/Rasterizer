#include <cmath>
#include "frustum.h"

using namespace aline;

Frustum::Frustum(real a) {
    alpha = ALPHA * PI / 180;
    real e = 1/std::tan(alpha/2);
    near = unit_vector(Vec4r {0, 0, -1, -N});
    far = unit_vector(Vec4r {0, 0, 1, F});
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}