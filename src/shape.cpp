#include "shape.h"

using namespace aline;

Shape::Shape(const std::string & name, const std::vector<Vertex> & vertices, const std::vector<Face> & faces):
    name(name), vertices(vertices), faces(faces) {}

std::string Shape::get_name() const { return name; }

std::vector<Vertex> Shape::get_vertices() const { return vertices; }
std::vector<Face> Shape::get_faces() const { return faces; }