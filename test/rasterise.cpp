#include <vector>
#include "scene.h"
#include "color.h"

using namespace aline;

int main(int argc, const char* argv[]) {
    Scene scene;

    scene.load_data(argc, argv);

    scene.initialise();
    scene.run();
    scene.shutdown();
}