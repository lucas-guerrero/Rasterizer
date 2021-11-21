#include "scene.h"

using namespace aline;

Scene::Scene(): isRunning(true) { shapes = std::vector<Shape>(); }

void Scene::add_shape(const Shape &shape) { shapes.push_back(shape); }

void Scene::initialise() {
    windows.set_title("Rasterizer");
    windows.set_height(CANVAS_HEIGHT);
    windows.set_width(CANVAS_WIDTH);

    if(!windows.load_font("fonts/FreeMonoBold.ttf"))
        std::cerr << "Couldn't load font." << std::endl;

    windows.register_quit_behavior( new QuitButtonBehavior( *this ) );
    windows.register_key_behavior( minwin::KEY_SPACE, new ChangeModeBehavior( *this ) );
}

void Scene::run() {

    minwin::Text modeText(10, CANVAS_HEIGHT - 25, "Mode: wireframe", minwin::white);

    minwin::Text spaceText(10, 10, "Press space to change mode", minwin::white);

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

                Vertex v1 = vertices[f.idP1];
                Vertex v2 = vertices[f.idP2];
                Vertex v3 = vertices[f.idP3];

                Vec2r p1 = v1.point;
                Vec2r p2 = v2.point;
                Vec2r p3 = v3.point;

                Vec2r canvasP1 = viewport_to_canvas(p1);
                Vec2r canvasP2 = viewport_to_canvas(p2);
                Vec2r canvasP3 = viewport_to_canvas(p3);

                Vec2i point1 = canvas_to_window(canvasP1);
                Vec2i point2 = canvas_to_window(canvasP2);
                Vec2i point3 = canvas_to_window(canvasP3);

                if(mode == 0) {
                    draw_wireframe_triangle(point1, point2, point3);
                    modeText.set_string("Mode: Wireframe");
                }
                else if(mode == 1) {
                    draw_filled_triangle(point1, point2, point3);
                    modeText.set_string("Mode: Filled");
                }
                else {
                    draw_shaded_triangle(point1, point2, point3, v1.intensity, v2.intensity, v3.intensity, f.color);
                    modeText.set_string("Mode: Shaded");
                }
            }
        }
        windows.render_text(modeText);
        windows.render_text(spaceText);
        
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

void Scene::draw_shaded_triangle(const Vec2i &v1, const Vec2i &v2, const Vec2i &v3, real h0, real h1, real h2, const minwin::Color &color) const{
    Vec2i p0 = Vec2i(v1);
    Vec2i p1 = Vec2i(v2);
    Vec2i p2 = Vec2i(v3);
    
    if(p1[1] < p0[1]) {
        std::swap(p1, p0);
        std::swap(h1, h0);
    }
        
    if(p2[1] < p0[1]) {
        std::swap(p2, p0);
        std::swap(h2, h0);
    }
        
    if(p2[1] < p1[1]) {
        std::swap(p2, p1);
        std::swap(h2, h1);
    }

    int x0 = p0[0], x1 = p1[0], x2 = p2[0];
    int y0 = p0[1], y1 = p1[1], y2 = p2[1];

    std::vector<uint> x02 = interpolation(y0, x0, y2, x2);
    std::vector<real> h02 = interpolation_shaded(y0, h0, y2, h2);

    std::vector<uint> x01 = interpolation(y0, x0, y1, x1);
    std::vector<real> h01 = interpolation_shaded(y0, h0, y1, h1);

    std::vector<uint> x12 = interpolation(y1, x1, y2, x2);
    std::vector<real> h12 = interpolation_shaded(y1, h1, y2, h2);
    
    x01.pop_back();
    x01.insert(x01.end(), x12.begin(), x12.end());

    h01.pop_back();
    h01.insert(h01.end(), h12.begin(), h12.end());

    std::vector<uint> x_left, x_right;
    std::vector<real> h_left, h_right;
    int m = (int)(std::floor(x01.size()/2));
    if(x02[m] < x01[m]) {
        x_left = x02;
        x_right = x01;

        h_left = h02;
        h_right = h01;
    }
    else {
        x_left = x01;
        x_right = x02;

        h_left = h01;
        h_right = h02;
    }

    double red = color.red;
    double green = color.green;
    double blue = color.blue;
    
    for(int y = y0; y <= y2; ++y) {
        int diff = y - y0;
        uint x_l = std::round(x_left[diff]);
        uint x_r = std::round(x_right[diff]);

        std::vector<real> h_segment = interpolation_shaded(x_l, h_left[diff], x_r, h_right[diff]);

        for(uint x = x_l; x <= x_r; ++x) {
            real intens = h_segment[x - x_l];
            minwin::Color c;
            c.red = red * intens;
            c.green = green * intens;
            c.blue = blue * intens;

            windows.put_pixel(x, y, c);
        }
    }
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
        uint x_l = std::round(x_left[diff]);
        uint x_r = std::round(x_right[diff]);
        for(uint x = x_l; x <= x_r; ++x) {
            windows.put_pixel(x, y);
        }
    }
}

real sign(const int value) {
    if(value < 0)
        return -1;
    return 1;
}

std::vector<real> Scene::interpolation_shaded(int i0, real h0, int i1, real h1) const {
    if(i0 == i1) return {h0};

    std::vector<real> listPoint;

    real hd = h1 - h0, di = i1 - i0;

    real a = hd / di;
    real d = h0;

    for(int i = i0; i <= i1; ++i){
        listPoint.push_back(d);
        d = d + a;
    }

    return listPoint;
}

std::vector<uint> Scene::interpolation(int i0, int d0, int i1, int d1) const {
    if(i0 == i1) return {(uint)d0};

    std::vector<uint> listPoint;

    int dd = d1 - d0, di = i1 - i0;

    real a = (real)dd / di;
    real d = d0;

    for(int i = i0; i <= i1; ++i){
        listPoint.push_back(std::round(d));
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

void Scene::changeMode() { mode = (mode+1) %3; }

void Scene::shutdown() { windows.close(); }

QuitButtonBehavior::QuitButtonBehavior( Scene & scene ) : owner(scene) {}

void QuitButtonBehavior::on_click() const { owner.quit(); }

ChangeModeBehavior::ChangeModeBehavior( Scene & scene ) : owner(scene) {}

void ChangeModeBehavior::on_press() const { owner.changeMode(); }

void ChangeModeBehavior::on_release() const {  }