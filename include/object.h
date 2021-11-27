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
    Vec3r barycentre;

public:
    Object(const Shape &shape, const Vec3r &translation = {}, const Vec3r &rotation = {}, const Vec3r &scale = {1, 1, 1});

    Matrix<real, 4, 4> transform() const;

    Matrix<real, 4, 4> matrixTranslation(const Vec3r &transl) const;
    Matrix<real, 4, 4> matrixScale() const;
    Matrix<real, 4, 4> matrixRotation() const;

    Matrix<real, 4, 4> matrixRotationX() const;
    Matrix<real, 4, 4> matrixRotationY() const;
    Matrix<real, 4, 4> matrixRotationZ() const;

    std::vector<Vertex> get_vertices() const;
    std::vector<Face> get_faces() const;
};

}