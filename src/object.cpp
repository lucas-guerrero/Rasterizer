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

Mat44r Object::transform() const{
    Mat44r matrix = matrixTranslation(-barycentre);

    matrix = matrixScale() * matrix;
    matrix = matrixRotation() * matrix;
    matrix = matrixTranslation(translation) * matrix;
    matrix = matrixTranslation(barycentre) * matrix;

    return matrix;
}


Mat44r Object::matrixRotation() const{
    Mat44r matrix = matrixRotationX();

    matrix = matrixRotationY() * matrix;
    matrix = matrixRotationZ() * matrix;

    return matrix;
}

Mat44r Object::matrixTranslation(const Vec3r &transl) const{
    return Mat44r {
        {1, 0, 0, transl[0]},
        {0, 1, 0, transl[1]},
        {0, 0, 1, transl[2]},
        {0, 0, 0, 1}
    };
}

Mat44r Object::matrixScale() const{
    return Mat44r {
        {scale[0], 0, 0, 0},
        {0, scale[1], 0, 0},
        {0, 0, scale[2], 0},
        {0, 0, 0, 1}
    };
}

Mat44r Object::matrixRotationX() const{
    real tmp = rotation[0]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Mat44r {
        {1, 0, 0, 0},
        {0, cos, -sin, 0},
        {0, sin, cos, 0},
        {0, 0, 0, 1}
    };
}

Mat44r Object::matrixRotationY() const{
    real tmp = rotation[1]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Mat44r {
        {cos, 0, sin, 0},
        {0, 1, 0, 0},
        {-sin, 0, cos, 0},
        {0, 0, 0, 1}
    };
}

Mat44r Object::matrixRotationZ() const{
    real tmp = rotation[2]*PI /180;
    real cos = std::cos(tmp);
    real sin = std::sin(tmp);

    return Mat44r {
        {cos, -sin, 0, 0},
        {sin, cos, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}