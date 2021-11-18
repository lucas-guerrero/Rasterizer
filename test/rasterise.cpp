#include "scene.h"

using namespace aline;

int main() {
    Scene scene;

    scene.initialise();
    scene.run();
    scene.shutdown();
}