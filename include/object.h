#pragma once
#include "shape.h"
#include "vector.h"
#include "matrix.h"

namespace aline {

class Object {

private:
    Shape shape;
    Vec3r translation;
    Vec3r rotation;
    Vec3r scale;

public:
    Object(const Shape &shape, const Vec3r &translation, const Vec3r &rotation, const Vec3r &scale);

    Matrix<real, 4, 4> transform();

    std::vector<Vertex> get_vertices();
    std::vector<Face> get_faces();
};

}