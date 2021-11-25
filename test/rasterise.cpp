#include <vector>
#include "scene.h"
#include "color.h"

using namespace aline;

int main(int argc, const char* argv[]) {
    Scene scene;

    scene.load_data(argc, argv);
/*
    std::vector<Vertex> listVertex {Vertex({0.6, 0}, 0)};
    std::vector<Face> listFace;

    const real PI = 3.141592653589;
    const real DIV = (real)1/50;

    int i = 1;
    for(real angle = 0; angle < 2*PI; angle += DIV*PI) {
        real x = std::cos(angle)/4 + 0.6;
        real y = std::sin(angle)/4 ;

        if(i%2 == 0)
            listVertex.push_back(Vertex({x, y}, 1));
        else
            listVertex.push_back(Vertex({x, y}, 0.5));

        listFace.push_back(Face(0, i, i-1, minwin::red));
        ++i;
    }

    listFace.push_back(Face(0, 1, i-1, minwin::red));

    Shape cercle = Shape("Cercle", listVertex, listFace);


    std::vector<Vertex> listVertex1 {Vertex({-0.6, 0.25}, 1), Vertex({-0.55, -0.1}, 0.5), Vertex({-0.8, -0.5}, 0)};
    std::vector<Face> listFace1 {Face(0, 1, 2, minwin::blue)};

    Shape triangle = Shape("Triangle", listVertex1, listFace1);

    std::vector<Vertex> listVertex2 {Vertex({-0.25, 0.25}, 1), Vertex({0.25, 0.25}, 0.5), Vertex({0.25, -0.25}, 0), Vertex({-0.25, -0.25}, 0)};
    std::vector<Face> listFace2 {Face(0, 1, 2, minwin::white), Face(0, 2, 3, minwin::white)};

    Shape carre = Shape("Carre", listVertex2, listFace2);

    scene.add_shape(triangle);
    scene.add_shape(carre);
    scene.add_shape(cercle);
*/

    scene.initialise();
    scene.run();
    scene.shutdown();
}