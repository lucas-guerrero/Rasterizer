#include "scene.h"

using namespace aline;

Scene::Scene(): isRunning(true) { shapes = std::vector<Shape>(); }

void Scene::add_shape(const Shape &shape) { shapes.push_back(shape); }

void Scene::initialise() {
    windows.set_title("Rasterizer");
    windows.set_height(CANVAS_HEIGHT);
    windows.set_width(CANVAS_WIDTH);

    windows.register_quit_behavior( new QuitButtonBehavior( *this ) );
}

void Scene::run() {
    if( not windows.open() ) {
        std::cerr << "Couldn't open window.\n";
    }

    const real PI = 3.141592653589;
    const real DIV = (real)1/20;

    while(isRunning) {
        windows.process_input();
        windows.set_draw_color(minwin::white);
        
        Vec2r mCanvas = viewport_to_canvas({0, 0});
        Vec2i mid = canvas_to_window(mCanvas);

        for(real angle = 0; angle < 2*PI; angle += DIV*PI) {
            real x = std::cos(angle);
            real y = std::sin(angle);

            Vec2r canvas = viewport_to_canvas({x, y});
            Vec2i point = canvas_to_window(canvas);
            //std::cout << millieu << ", " << point << std::endl; 
            drawLine({200, 300}, point);
            //windows.put_pixel(point[0], point[1]);
        }

        windows.put_pixel(mid[0], mid[1], minwin::red);
        windows.display();
    }
}

Vec2r Scene::viewport_to_canvas( const Vec2r & point ) const {
    real cX = ( point[0] * CANVAS_WIDTH ) / VIEWPORT_WIDTH;
    real cY = ( point[1] * CANVAS_HEIGHT ) / VIEWPORT_HEIGHT;
    return Vec2r {cX, cY};
}

Vec2i Scene::canvas_to_window( const Vec2r & point ) const {
    int sX = CANVAS_WIDTH / 2 + point[0];
    int sY = CANVAS_HEIGHT / 2 - point[1];
    return Vec2i {sX, sY};
}

real sign(const int value) {
    if(value < 0)
        return -1;
    return 1;
}

// Algo incremental
void Scene::drawLine(const Vec2i &v1, const Vec2i &v2) {
    int x0 = v1[0], y0 = v1[0], x1 = v2[0], y1 = v2[1];

    int dx = x1 - x0, dy = y1 - y0;
    if(std::abs(dx) > std::abs(dy)) {
        real a;
        if(dx == 0)
            a = sign(dy);
        else {
            if(x0 > x1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            a = (real)dy/dx;
        }
        real y = y0;
        for(int x = x0; x <= x1; ++x) {
            windows.put_pixel(x, std::round(y));
            y = y + a;
        }
    }
    else {
        real a;
        if(dy == 0)
            a = sign(dx);
        else {
            if(y0 > y1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            a = (real)dx/dy;
        }
        
        real x = x0;
        for(int y = y0; y <= y1; ++y) {
            windows.put_pixel(std::round(x), y);
            x = x + a;
        }
    }
}

/*
//Algo de Bresenham
void Scene::drawLine(const Vec2i &v1, const Vec2i &v2) {
    int x0 = v1[0], y0 = v1[0], x1 = v2[0], y1 = v2[1];

    int dx = x1 - x0, dy = y1 - y0;
    int ax = dx << 1, ay = dy << 1;
    int d = ay - ax;

    if(std::abs(dx) > std::abs(dy)) {
        if(x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        for(int x = x0, y = y0; x <= x1; ++x) {
            windows.put_pixel(x, y);
            if(d >= 0) {
                ++y;
                d = d - ax;
            }
            d = d + ay;
        }
    }
    else {
        d = ax - ay;
        if(y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        for(int y = y0, x = x0; y <= y1; ++y) {
            windows.put_pixel(x, y);
            if(d >= 0) {
                ++x;
                d = d - ax;
            }
            d = d + ay;
        }
    }

    
}
*/

void Scene::quit() { isRunning = false; }

void Scene::shutdown() { windows.close(); }

QuitButtonBehavior::QuitButtonBehavior( Scene & scene ) : owner(scene) {}

void QuitButtonBehavior::on_click() const { this->owner.quit(); }