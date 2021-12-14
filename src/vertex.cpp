#include "vertex.h"

using namespace aline;

Vertex::Vertex(const Vec3r &c, real h): point(c), intensity(h) {}

Vertex::Vertex(): point({}), intensity(1) {}