#include <vector>
#include "scene.h"

using namespace aline;

int main() {
    Scene scene;

    std::vector<Vertex> listVertex {Vertex({0, 0}, 1)};
    std::vector<Face> listFace;

    const real PI = 3.141592653589;
    const real DIV = (real)1/100;

    int i = 1;
    for(real angle = 0; angle < 2*PI; angle += DIV*PI) {
        real x = std::cos(angle);
        real y = std::sin(angle);

        listVertex.push_back(Vertex({x, y}, 1));
        listFace.push_back(Face(0, i, i-1));
        ++i;
    }

    Shape shape = Shape("Cercle", listVertex, listFace);

    scene.add_shape(shape);
    scene.initialise();
    scene.run();
    scene.shutdown();
}