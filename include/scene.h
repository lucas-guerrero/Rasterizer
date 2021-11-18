#pragma once
#include <vector>
#include "shape.h"

#include "color.h"
#include "ibehavior.h"
#include "text.h"
#include "window.h"

namespace aline {

class Scene {
    const uint HEIGHT = 600;
    const uint WIDTH = 400;

    std::vector<Shape> shapes;
    minwin::Window windows;
    bool isRunning;

public:
    Scene();
    void add_shape(const Shape& shape);
    void initialise();
    void run();
    void quit();
    void shutdown();
};

class QuitButtonBehavior : public minwin::IButtonBehavior
  {
    public:
      QuitButtonBehavior( Scene & scene ) : owner { scene } {}
      void on_click() const override;
    private:
      Scene & owner;
  };

}