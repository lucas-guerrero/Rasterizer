#include <cmath>
#include "frustum.h"

using namespace aline;

Frustum::Frustum(real dNear, real dFar, real a, real e):dNear(dNear), dFar(dFar)  {
    near = unit_vector(Vec4r {0, 0, -1, -dNear});
    far = unit_vector(Vec4r {0, 0, 1, dFar});
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}

void Frustum::update(real a, real e) {
    near = unit_vector(Vec4r {0, 0, -1, -dNear});
    far = unit_vector(Vec4r {0, 0, 1, dFar});
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}