#pragma once
#include <vector>
#include "shape.h"

#include "color.h"
#include "ibehavior.h"
#include "text.h"
#include "window.h"

namespace aline {

class Scene {
  const uint CANVAS_HEIGHT = 600;
  const uint CANVAS_WIDTH = 400;
  const real RATIO = CANVAS_HEIGHT/CANVAS_WIDTH;
  const real VIEWPORT_WIDTH = 2;
  const real VIEWPORT_HEIGHT = VIEWPORT_WIDTH * RATIO;

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

private:
  Vec2r viewport_to_canvas( const Vec2r & point ) const;
  Vec2i canvas_to_window( const Vec2r & point ) const;
  void drawLine(const Vec2i &v1, const Vec2i &v2);
};

class QuitButtonBehavior : public minwin::IButtonBehavior {
public:
  QuitButtonBehavior( Scene & scene );
  void on_click() const override;
private:
  Scene & owner;
};

}