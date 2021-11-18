#include "scene.h"

using namespace aline;

Scene::Scene(): isRunning(true) { shapes = std::vector<Shape>(); }

void Scene::add_shape(const Shape &shape) { shapes.push_back(shape); }

void Scene::initialise() {
    windows.set_title("Rasterizer");
    windows.set_height(HEIGHT);
    windows.set_width(WIDTH);

    windows.register_quit_behavior( new QuitButtonBehavior( *this ) );
}

void Scene::run() {
    if( not windows.open() ) {
        std::cerr << "Couldn't open window.\n";
    }

    while(isRunning) {
        windows.process_input();
    }
}

void Scene::quit() { isRunning = false; }

void Scene::shutdown() { windows.close(); }

void QuitButtonBehavior::on_click() const { this->owner.quit(); }