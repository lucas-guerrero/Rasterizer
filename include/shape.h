#pragma once
#include <string>
#include <vector>
#include "face.h"
#include "vertex.h"

namespace aline {

class Shape {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

public:
    Shape(const std::string & name, const std::vector<Vertex> & vertices, const std::vector<Face> & faces);

    std::string get_name() const;
    std::vector<Vertex> get_vertices() const;
    std::vector<Face> get_faces() const;
};

}