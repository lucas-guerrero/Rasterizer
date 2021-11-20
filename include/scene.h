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
  const uint CANVAS_WIDTH = 600;
  const real RATIO = CANVAS_HEIGHT/CANVAS_WIDTH;
  const real VIEWPORT_WIDTH = 2;
  const real VIEWPORT_HEIGHT = VIEWPORT_WIDTH * RATIO;

  std::vector<Shape> shapes;
  minwin::Window windows;
  bool isRunning;
  uint mode = 0; // 0 = wireframe, 1 = filled, 2 = shaded

public:
  Scene();
  void add_shape(const Shape& shape);
  void initialise();
  void run();
  void quit();
  void changeMode();
  void shutdown();

private:
  Vec2r viewport_to_canvas( const Vec2r & point ) const;
  Vec2i canvas_to_window( const Vec2r & point ) const;
  void draw_line(const Vec2i &v1, const Vec2i &v2) const;
  void draw_wireframe_triangle(const Vec2i &v1, const Vec2i &v2, const Vec2i &v3) const;
  void draw_filled_triangle(const Vec2i &v1, const Vec2i &v2, const Vec2i &v3) const;
  std::vector<uint> interpolation(int i0, int d0, int i1, int d1) const;
};

class QuitButtonBehavior : public minwin::IButtonBehavior {
public:
  QuitButtonBehavior( Scene & scene );
  void on_click() const override;
private:
  Scene & owner;
};

class ChangeModeBehavior : public minwin::IKeyBehavior {
public:
  ChangeModeBehavior( Scene & scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene & owner;
};

}