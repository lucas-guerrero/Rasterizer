#include "object.h"

using namespace aline;

Object::Object(const Shape &shape, const Vec3r &translation, const Vec3r &rotation, const Vec3r &scale):
    shape(shape), translation(translation), rotation(rotation), scale(scale) {
        real x = 0, y = 0, z = 0;

        for(const auto elm: shape.get_vertices()) {
            x += elm.point[0];
            y += elm.point[1];
            z += elm.point[2];
        }

        real t = shape.get_vertices().size();

        barycentre = Vec3r {x/t, y/t, z/t};
    }

std::vector<Vertex> Object::get_vertices() const { return shape.get_vertices(); }

std::vector<Face> Object::get_faces() const { return shape.get_faces(); }

Matrix<real, 4, 4> Object::transform() const{
    Matrix<real, 4, 4> matrix = matrixTranslation(-barycentre);

    matrix = matrixScale() * matrix;
    matrix = matrixRotation() * matrix;
    matrix = matrixTranslation(translation) * matrix;
    matrix = matrixTranslation(barycentre) * matrix;

    return matrix;
}


Matrix<real, 4, 4> Object::matrixRotation() const{
    Matrix<real, 4, 4> matrix = matrixRotationX();

    matrix = matrixRotationY() * matrix;
    matrix = matrixRotationZ() * matrix;

    return matrix;
}

Matrix<real, 4, 4> Object::matrixTranslation(const Vec3r &transl) const{
    return Matrix<real, 4, 4> {
        {1, 0, 0, transl[0]},
        {0, 1, 0, transl[1]},
        {0, 0, 1, transl[2]},
        {0, 0, 0, 1}
    };
}

Matrix<real, 4, 4> Object::matrixScale() const{
    return Matrix<real, 4, 4> {
        {scale[0], 0, 0, 0},
        {0, scale[1], 0, 0},
        {0, 0, scale[2], 0},
        {0, 0, 0, 1}
    };
}

Matrix<real, 4, 4> Object::matrixRotationX() const{
    real tmp = rotation[0]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Matrix<real, 4, 4> {
        {1, 0, 0, 0},
        {0, cos, -sin, 0},
        {0, sin, cos, 0},
        {0, 0, 0, 1}
    };
}

Matrix<real, 4, 4> Object::matrixRotationY() const{
    real tmp = rotation[1]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Matrix<real, 4, 4> {
        {cos, 0, sin, 0},
        {0, 1, 0, 0},
        {-sin, 0, cos, 0},
        {0, 0, 0, 1}
    };
}

Matrix<real, 4, 4> Object::matrixRotationZ() const{
    real tmp = rotation[2]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Matrix<real, 4, 4> {
        {cos, -sin, 0, 0},
        {sin, cos, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}