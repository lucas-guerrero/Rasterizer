#include "scene.h"

using namespace aline;

Scene::Scene(): isRunning(true) { shapes = std::vector<Shape>(); }

void Scene::add_shape(const Shape &shape) { shapes.push_back(shape); }

void Scene::initialise() {
    windows.set_title("Rasterizer");
    windows.set_height(CANVAS_HEIGHT);
    windows.set_width(CANVAS_WIDTH);

    windows.register_quit_behavior( new QuitButtonBehavior( *this ) );
    windows.register_key_behavior( minwin::KEY_SPACE, new ChangeModeBehavior( *this ) );
}

void Scene::run() {
    if( not windows.open() ) {
        std::cerr << "Couldn't open window.\n";
    }

    while(isRunning) {
        windows.process_input();

        windows.clear();
        
        for(uint i = 0; i<shapes.size(); ++i) {
            std::vector<Vertex> vertices = shapes[i].get_vertices();
            std::vector<Face> faces = shapes[i].get_faces();

            for(uint j=0; j<faces.size(); ++j) {
                Face f = faces[j];

                windows.set_draw_color(f.color);

                Vec2r p1 = vertices[f.idP1].point;
                Vec2r p2 = vertices[f.idP2].point;
                Vec2r p3 = vertices[f.idP3].point;

                Vec2r canvasP1 = viewport_to_canvas(p1);
                Vec2r canvasP2 = viewport_to_canvas(p2);
                Vec2r canvasP3 = viewport_to_canvas(p3);

                Vec2i point1 = canvas_to_window(canvasP1);
                Vec2i point2 = canvas_to_window(canvasP2);
                Vec2i point3 = canvas_to_window(canvasP3);

                if(mode == 0)
                    draw_wireframe_triangle(point1, point2, point3);
                else if(mode == 1)
                    draw_filled_triangle(point1, point2, point3);
                else
                    draw_line({600, 0}, {0, 600}); 
            }
        }
        
        windows.display();
    }
}

Vec2r Scene::viewport_to_canvas( const Vec2r & point ) const {
    real cX = ( point[0] * CANVAS_WIDTH ) / VIEWPORT_WIDTH;
    real cY = ( point[1] * CANVAS_HEIGHT ) / VIEWPORT_HEIGHT;
    return Vec2r {cX, cY};
}

Vec2i Scene::canvas_to_window( const Vec2r & point ) const {
    int sX = std::round(CANVAS_WIDTH / 2 + point[0]);
    int sY = std::round(CANVAS_HEIGHT / 2 - point[1]);
    return Vec2i {sX, sY};
}

void Scene::draw_wireframe_triangle(const Vec2i &v1, const Vec2i &v2, const Vec2i &v3) const {
    draw_line(v1, v2);
    draw_line(v2, v3);
    draw_line(v3, v1);
}

void print(const std::vector<uint> &v) {
    for(const auto elm: v) {
        std::cout << elm << ", ";
    }
    std::cout << std::endl;
}

void Scene::draw_filled_triangle(const Vec2i &v1, const Vec2i &v2, const Vec2i &v3) const {
    Vec2i p0 = Vec2i(v1);
    Vec2i p1 = Vec2i(v2);
    Vec2i p2 = Vec2i(v3);
    
    if(p1[1] < p0[1])
        std::swap(p1, p0);
    if(p2[1] < p0[1])
        std::swap(p2, p0);
    if(p2[1] < p1[1])
        std::swap(p2, p1);

    int x0 = p0[0], x1 = p1[0], x2 = p2[0];
    int y0 = p0[1], y1 = p1[1], y2 = p2[1];

    std::vector<uint> x02 = interpolation(y0, x0, y2, x2);
    std::vector<uint> x01 = interpolation(y0, x0, y1, x1);
    std::vector<uint> x12 = interpolation(y1, x1, y2, x2);


    
    x01.pop_back();

    x01.insert(x01.end(), x12.begin(), x12.end());

    std::vector<uint> x_left, x_right;
    int m = (int)(std::floor(x01.size()/2));
    if(x02[m] < x01[m]) {
        x_left = x02;
        x_right = x01;
    }
    else {
        x_left = x01;
        x_right = x02;
    }
    
    for(int y = y0; y <= y2; ++y) {
        int diff = y - y0;
        for(uint x = x_left[diff]; x <= x_right[diff]; ++x) {
            windows.put_pixel(x, y);
        }
    }
}

real sign(const int value) {
    if(value < 0)
        return -1;
    return 1;
}

std::vector<uint> Scene::interpolation(int i0, int d0, int i1, int d1) const {
    if(i0 == i1) return {(uint)d0};

    std::vector<uint> listPoint;

    int dd = d1 - d0, di = i1 - i0;

    real a = (real)dd / di;
    real d = d0;

    //std::cout << std::endl;
    //std::cout << "i0: " << i0 << ", d0: " << d0 << ", i1: " << i1 << ", d1: " << d1 << std::endl;
    //std::cout << "dd: " << dd << ", di: " << di << ", a: " << a << std::endl;

    for(int i = i0; i <= i1; ++i){
        listPoint.push_back(std::round(d));
        //std::cout << "push: " << std::round(d) << std::endl;
        d = d + a;
    }

    return listPoint;
}

void Scene::draw_line(const Vec2i &v1, const Vec2i &v2) const {
    int x0 = v1[0], y0 = v1[1], x1 = v2[0], y1 = v2[1];

    if(v1 == v2) {
        windows.put_pixel(x0, y0);
        return;
    }

    int dx = x1 - x0, dy = y1 - y0;
    int ax = dx << 1, ay = dy << 1;

    if(std::abs(dx) > std::abs(dy)) {
        if(x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0, dy = y1 - y0;
            ax = dx << 1, ay = dy << 1;
        }
        if(y0 > y1) {
            dx = x1 - x0, dy = y0 - y1;
            ax = dx << 1, ay = dy << 1;
        }

        int d = ay - ax;
        
        for(int x = x0, y = y0; x <= x1; ++x) {
            windows.put_pixel(x, y);
            if(d >= 0) {
                if(y0 > y1)
                    --y;
                else
                    ++y;
                d = d - ax;
            }
            d = d + ay;
        }
    }
    else {
        if(y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0, dy = y1 - y0;
            ax = dx << 1, ay = dy << 1;
        }
        if(x0 > x1) {
            dx = x0 - x1, dy = y1 - y0;
            ax = dx << 1, ay = dy << 1; 
        }

        int d = ax - ay;

        for(int y = y0, x = x0; y <= y1; ++y) {
            windows.put_pixel(x, y);
            if(d >= 0) {
                if(x0 > x1)
                    --x;
                else
                    ++x;
                d = d - ay;
            }
            d = d + ax;
        }
    }
}

void Scene::quit() { isRunning = false; }

void Scene::changeMode() { mode = (mode+1) %2; }

void Scene::shutdown() { windows.close(); }

QuitButtonBehavior::QuitButtonBehavior( Scene & scene ) : owner(scene) {}

void QuitButtonBehavior::on_click() const { owner.quit(); }

ChangeModeBehavior::ChangeModeBehavior( Scene & scene ) : owner(scene) {}

void ChangeModeBehavior::on_press() const { owner.changeMode(); }

void ChangeModeBehavior::on_release() const {  }