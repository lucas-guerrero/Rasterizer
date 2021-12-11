#include "vectorAxis.h"

using namespace aline;

Vec3r Forward::X = {1, 0, 0};
Vec3r Forward::Y = {0, 1, 0};
Vec3r Forward::Z = {0, 0, -1};

Vec3r Backward::X = {-1, 0, 0};
Vec3r Backward::Y = {0, -1, 0};
Vec3r Backward::Z = {0, 0, 1};

Vec3r Clockwise::X = {-1, 0, 0};
Vec3r Clockwise::Y = {0, -1, 0};
Vec3r Clockwise::Z = {0, 0, -1};

Vec3r AntiClockwise::X = {1, 0, 0};
Vec3r AntiClockwise::Y = {0, 1, 0};
Vec3r AntiClockwise::Z = {0, 0, 1};