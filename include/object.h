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

    Mat44r transform() const;

    Mat44r matrixTranslation(const Vec3r &transl) const;
    Mat44r matrixScale() const;
    Mat44r matrixRotation() const;

    Mat44r matrixRotationX() const;
    Mat44r matrixRotationY() const;
    Mat44r matrixRotationZ() const;

    std::vector<Vertex> get_vertices() const;
    std::vector<Face> get_faces() const;
};

}