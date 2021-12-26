#pragma once
#include <vector>
#include "object.h"
#include "camera.h"

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

  Camera camera;
  std::vector<Object> objects;
  
  minwin::Window windows;
  bool isRunning;
  uint mode = 0; // 0 = wireframe, 1 = filled, 2 = shaded

  minwin::Text modeText;

  real **depth_buffer;

public:
  Scene();
  inline Camera &getCamera() { return camera; }
  void load_data( int argc, const char * argv[] );
  void load_obj_file( const char * file_name );
  void add_object(const Object& object);
  void initialise();
  void run();
  void quit();
  void changeMode();
  void shutdown();

private:

  void draw_object(const Object& object);

  Object apply_object_transform(const Object& object);
  Object apply_camera_transform(const Object& object);

  Object cull_and_clip(const Object& object);

  Vec3r perspective_projection(const Vec4r &v, real d);
  Vec3r viewport_to_canvas( const Vec3r & point ) const;
  Vec3r canvas_to_window( const Vec3r & point ) const;
  void draw_line(const Vec3r &v1, const Vec3r &v2) const;
  void draw_wireframe_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3) const;
  void draw_shaded_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3, real h0, real h1, real h2, const minwin::Color& color) const;
  void draw_filled_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3) const;
  std::vector<real> interpolation(int i0, real h0, int i1, real h1) const;

  void initDepthBuffer();
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


class ForwardXBehavior : public minwin::IKeyBehavior {
public:
  ForwardXBehavior( Scene & scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene & owner;
};

class BackwardXBehavior : public minwin::IKeyBehavior {
public:
  BackwardXBehavior( Scene & scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene & owner;
};


class ForwardZBehavior : public minwin::IKeyBehavior {
public:
  ForwardZBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

class BackwardZBehavior : public minwin::IKeyBehavior {
public:
  BackwardZBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};


class ForwardYBehavior : public minwin::IKeyBehavior {
public:
  ForwardYBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

class BackwardYBehavior : public minwin::IKeyBehavior {
public:
  BackwardYBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};


class CwXBehavior : public minwin::IKeyBehavior {
public:
  CwXBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

class AcwXBehavior : public minwin::IKeyBehavior {
public:
  AcwXBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};


class CwYBehavior : public minwin::IKeyBehavior {
public:
  CwYBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

class AcwYBehavior : public minwin::IKeyBehavior {
public:
  AcwYBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};


class CwZBehavior : public minwin::IKeyBehavior {
public:
  CwZBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

class AcwZBehavior : public minwin::IKeyBehavior {
public:
  AcwZBehavior( Scene &scene );
  void on_press() const override;
  void on_release() const override;
private:
  Scene &owner;
};

}