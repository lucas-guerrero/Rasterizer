#include <vector>
#include "scene.h"
#include "color.h"

using namespace aline;

int main() {
    Scene scene;

    std::vector<Vertex> listVertex {Vertex({0, 0}, 0)};
    std::vector<Face> listFace;

    const real PI = 3.141592653589;
    const real DIV = (real)1/50;

    int i = 1;
    for(real angle = 0; angle < 2*PI; angle += DIV*PI) {
        real x = std::cos(angle) /2;
        real y = std::sin(angle) /2;

        listVertex.push_back(Vertex({x, y}, 1));
        listFace.push_back(Face(0, i, i-1, minwin::red));
        ++i;
    }

    listFace.push_back(Face(0, 1, i-1, minwin::red));


    //std::vector<Vertex> listVertex {Vertex({0, 0}, 1), Vertex({-0.2, -0.4}, 1), Vertex({0.2, -0.2}, 1)};
    //std::vector<Vertex> listVertex {Vertex({0, 0}, 1), Vertex({-0.5, -0.5}, 1), Vertex({0.5, -0.5}, 1)};
    //std::vector<Face> listFace {Face(0, 1, 2, minwin::red)};

    Shape shape = Shape("Cercle", listVertex, listFace);

    scene.add_shape(shape);
    scene.initialise();
    scene.run();
    scene.shutdown();
}