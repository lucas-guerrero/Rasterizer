#include "object.h"

using namespace aline;

Object::Object(const Shape &shape, const Vec3r &translation, const Vec3r &rotation, const Vec3r &scale):
    shape(shape), translation(translation), rotation(rotation), scale(scale) {}

std::vector<Vertex> Object::get_vertices() { return shape.get_vertices(); }

std::vector<Face> Object::get_faces() { return shape.get_faces(); }

Matrix<real, 4, 4> Object::transform() {
    return Matrix<real, 4, 4> {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}