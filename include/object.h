#pragma once
#include <math.h>
#include "shape.h"
#include "vector.h"
#include "matrix.h"

#define PI 3.14159265

namespace aline {

class Object {

public:
    Shape shape;
    Vec3r translation;
    Vec3r rotation;
    Vec3r scale;

public:
    Object(const Shape &shape, const Vec3r &translation = {}, const Vec3r &rotation = {}, const Vec3r &scale = {1, 1, 1});

    Matrix<real, 4, 4> transform();

    Matrix<real, 4, 4> matrixTranslation(const Vec3r &transl);
    Matrix<real, 4, 4> matrixScale();
    Matrix<real, 4, 4> matrixRotation();

    Matrix<real, 4, 4> matrixRotationX();
    Matrix<real, 4, 4> matrixRotationY();
    Matrix<real, 4, 4> matrixRotationZ();

    std::vector<Vertex> get_vertices();
    std::vector<Face> get_faces();

    Vec3r get_barycentre();
};

}