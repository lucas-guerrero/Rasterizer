#include <fstream>
#include <string>
#include <vector>
#include "scene.h"
#include "vectorAxis.h"

using namespace aline;

Scene::Scene(): camera(RATIO), isRunning(true) { 
    objects = std::vector<Object>();
    depth_buffer = new real*[600];
    for(int i=0; i<600; ++i)
        depth_buffer[i] = new real[600];
}

void Scene::load_data( int argc, const char * argv[] ) {
    for(int i = 1; i<argc; ++i) {
        load_obj_file(argv[i]);
    }
}

void Scene::load_obj_file( const char * file_name ) {
    std::ifstream file(file_name);
    if(file.fail())
        throw std::runtime_error("Can't dont open the file: " + std::string(file_name));

    std::string name;

    std::vector<Vertex> listVertex;
    std::vector<Face> listFace;

    int cpt = 0;

    while(file.good()) {
        std::string variable;

        file >> variable;

        if(file.fail()) break;

        if(variable == "#")
            std::getline(file, variable);
        else if(variable == "g")
            file >> name;
        else if(variable == "v") {
            real x, y, z;
            file >> x >> y >> z;
            if(cpt%2 == 0)
                listVertex.push_back(Vertex({x, y, z}, 1));
            else
                listVertex.push_back(Vertex({x, y, z}, 0));
            ++cpt;
        }
        else if(variable == "f") {
            std::string s1, s2, s3;
            file >> s1 >> s2 >> s3;

            uint i1 = std::stoul(s1.substr(0, s1.find("/")));
            uint i2 = std::stoul(s2.substr(0, s2.find("/")));
            uint i3 = std::stoul(s3.substr(0, s3.find("/")));

            listFace.push_back(Face(i1-1, i2-1, i3-1, minwin::white));
        }
    }

    Shape shape = Shape(name, listVertex, listFace);

    // Transformation for teapot
    add_object(Object(shape, {-5, -5, -50}, {}, {0.2, 0.2, 0.2}));

    // Transformation for tetrahedron
    // add_object(Object(shape, -1, {-1, -1, -5}, {0, 0, 0}));

    file.close();
}

void Scene::add_object(const Object &object) { objects.push_back(object); }

void Scene::initialise() {

    windows.set_title("Rasterizer");
    windows.set_height(CANVAS_HEIGHT);
    windows.set_width(CANVAS_WIDTH);

    if(!windows.load_font("fonts/FreeMonoBold.ttf"))
        std::cerr << "Couldn't load font." << std::endl;

    windows.register_quit_behavior( new QuitButtonBehavior( *this ) );
    windows.register_key_behavior( minwin::KEY_SPACE, new ChangeModeBehavior( *this ) );

    windows.register_key_behavior(minwin::KEY_K, new CwXBehavior(*this));
    windows.register_key_behavior(minwin::KEY_I, new AcwXBehavior(*this));

    windows.register_key_behavior(minwin::KEY_L, new CwYBehavior(*this));
    windows.register_key_behavior(minwin::KEY_J, new AcwYBehavior(*this));

    windows.register_key_behavior(minwin::KEY_O, new CwZBehavior(*this));
    windows.register_key_behavior(minwin::KEY_U, new AcwZBehavior(*this));

    windows.register_key_behavior(minwin::KEY_Q, new BackwardXBehavior(*this));
    windows.register_key_behavior(minwin::KEY_D, new ForwardXBehavior(*this));

    windows.register_key_behavior(minwin::KEY_S, new BackwardZBehavior(*this));
    windows.register_key_behavior(minwin::KEY_Z, new ForwardZBehavior(*this));

    windows.register_key_behavior(minwin::KEY_E, new BackwardYBehavior(*this));
    windows.register_key_behavior(minwin::KEY_A, new ForwardYBehavior(*this));

    if( not windows.open() ) {
        std::cerr << "Couldn't open window.\n";
    }
}

void Scene::initDepthBuffer() {
    for(int i=0; i<600; ++i)
        for(int j=0; j<600; ++j)
            depth_buffer[i][j] = 0;
}

void Scene::run() {

    modeText = minwin::Text(10, CANVAS_HEIGHT - 25, "Mode: wireframe", minwin::white);

    minwin::Text spaceText(10, 10, "Press space to change mode", minwin::white);

    while(isRunning) {
        windows.process_input();

        windows.clear();

        initDepthBuffer();
        for(uint i = 0; i<objects.size(); ++i) {
            draw_object(objects[i]);
        }

        camera.update();
        
        windows.render_text(modeText);
        windows.render_text(spaceText);
        
        windows.display();
    }
}

Vec3r Scene::perspective_projection(const Vec4r &v, real d) {
    real f = camera.distanceMax;
    Mat44r matrixProj = {
        {d, 0, 0, 0},
        {0, d, 0, 0},
        {0, 0, -(f+d)/(f-d), -(2*d*f)/(f-d)},
        {0, 0, -1, 0}
    };

    Vec4r vecProj = matrixProj * v;

    real x = vecProj[0] / vecProj[3];
    real y = vecProj[1] / vecProj[3];
    real z = vecProj[2] / vecProj[3];

    return {x, y, z};
}

Vec3r Scene::viewport_to_canvas( const Vec3r & point ) const {
    real cX = ( point[0] * CANVAS_WIDTH ) / VIEWPORT_WIDTH;
    real cY = ( point[1] * CANVAS_HEIGHT ) / VIEWPORT_HEIGHT;
    real cZ = ( point[2] * (camera.distanceMax - camera.focalDistance) ) / 2;
    return {cX, cY, cZ};
}

Vec3r Scene::canvas_to_window( const Vec3r & point ) const {
    real sX = CANVAS_WIDTH / 2 + point[0];
    real sY = CANVAS_HEIGHT / 2 - point[1];
    real sZ = (camera.distanceMax - camera.focalDistance) / 2 + point[2];
    return {sX, sY, sZ};
}

Object Scene::apply_object_transform(const Object& object) {
    Mat44r matrixTransform = object.transform();

    std::vector<Vertex> vertices;

    for(const auto vertex: object.get_vertices()) {

        Vec3r pp = vertex.point;
        Vec4r p = {pp[0], pp[1], pp[2], 1};

        Vec4r point = matrixTransform * p;

        vertices.push_back(Vertex({point[0], point[1], point[2]}, vertex.intensity));
    }

    Shape shape = Shape(object.shape.get_name(), vertices, object.get_faces());

    return Object(shape);
}

Object Scene::apply_camera_transform(const Object& object) {
    Mat44r matrixTransform = camera.transform();
/*
    Mat44r matrixBarycentreMoins = object.matrixTranslation(-object.barycentre);
    Mat44r matrixBarycentre = object.matrixTranslation(object.barycentre);

    matrixTransform = matrixBarycentre * matrixTransform * matrixBarycentreMoins;
*/
    std::vector<Vertex> vertices;

    for(const auto vertex: object.get_vertices()) {

        Vec3r pp = vertex.point;
        Vec4r p = {pp[0], pp[1], pp[2], 1};

        Vec4r point = matrixTransform * p;

        vertices.push_back(Vertex({point[0], point[1], point[2]}, vertex.intensity));
    }

    Shape shape = Shape(object.shape.get_name(), vertices, object.get_faces());

    return Object(shape);
}

Object Scene::cull_and_clip(const Object& object) {
    Object cull = camera.culling(object);
    return camera.fov.clipping(cull);
}

void Scene::draw_object(const Object &object) {
    
    Object o_transform = apply_object_transform(object);
    Object o_camera = apply_camera_transform(o_transform);
    Object o = cull_and_clip(o_camera);

    std::vector<Vertex> vertices = o.get_vertices();
    std::vector<Face> faces = o.get_faces();

    for(uint j=0; j<faces.size(); ++j) {
        Face f = faces[j];

        windows.set_draw_color(f.color);

        Vertex v1 = vertices[f.idP1];
        Vertex v2 = vertices[f.idP2];
        Vertex v3 = vertices[f.idP3];

        Vec4r p3_1 = {v1.point[0], v1.point[1], v1.point[2], 1};
        Vec4r p3_2 = {v2.point[0], v2.point[1], v2.point[2], 1};
        Vec4r p3_3 = {v3.point[0], v3.point[1], v3.point[2], 1};

        Vec3r p1 = perspective_projection(p3_1, 2);
        Vec3r p2 = perspective_projection(p3_2, 2);
        Vec3r p3 = perspective_projection(p3_3, 2);

        Vec3r canvasP1 = viewport_to_canvas(p1);
        Vec3r canvasP2 = viewport_to_canvas(p2);
        Vec3r canvasP3 = viewport_to_canvas(p3);

        Vec3r point1 = canvas_to_window(canvasP1);
        Vec3r point2 = canvas_to_window(canvasP2);
        Vec3r point3 = canvas_to_window(canvasP3);

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
            modeText.set_string("Mode: Shading");
        }
    }
}

void Scene::draw_wireframe_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3) const {
    draw_line(v1, v2);
    draw_line(v2, v3);
    draw_line(v3, v1);
}

void Scene::draw_shaded_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3, real h0, real h1, real h2, const minwin::Color &color) const{
    Vec3r p0 = Vec3r(v1);
    Vec3r p1 = Vec3r(v2);
    Vec3r p2 = Vec3r(v3);
    
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

    int x0 = std::round(p0[0]), x1 = std::round(p1[0]), x2 = std::round(p2[0]);
    int y0 = std::round(p0[1]), y1 = std::round(p1[1]), y2 = std::round(p2[1]);
    real z0 = p0[2], z1 = p1[2], z2 = p2[2];

    std::vector<real> x02 = interpolation(y0, x0, y2, x2);
    std::vector<real> h02 = interpolation(y0, h0, y2, h2);
    std::vector<real> z02 = interpolation(y0, z0, y2, z2);

    std::vector<real> x01 = interpolation(y0, x0, y1, x1);
    std::vector<real> h01 = interpolation(y0, h0, y1, h1);
    std::vector<real> z01 = interpolation(y0, z0, y1, z1);

    std::vector<real> x12 = interpolation(y1, x1, y2, x2);
    std::vector<real> h12 = interpolation(y1, h1, y2, h2);
    std::vector<real> z12 = interpolation(y1, z1, y2, z2);
    
    x01.pop_back();
    x01.insert(x01.end(), x12.begin(), x12.end());

    h01.pop_back();
    h01.insert(h01.end(), h12.begin(), h12.end());

    z01.pop_back();
    z01.insert(z01.end(), z12.begin(), z12.end());

    std::vector<real> x_left, x_right;
    std::vector<real> h_left, h_right;
    std::vector<real> z_left, z_right;
    int m = (int)(std::floor(x01.size()/2));
    if(x02[m] < x01[m]) {
        x_left = x02;
        x_right = x01;

        h_left = h02;
        h_right = h01;

        z_left = z02;
        z_right = z01;
    }
    else {
        x_left = x01;
        x_right = x02;

        h_left = h01;
        h_right = h02;

        z_left = z01;
        z_right = z02;
    }

    double red = color.red;
    double green = color.green;
    double blue = color.blue;
    
    for(int y = y0; y <= y2; ++y) {
        int diff = y - y0;
        uint x_l = std::round(x_left[diff]);
        uint x_r = std::round(x_right[diff]);

        std::vector<real> h_segment = interpolation(x_l, h_left[diff], x_r, h_right[diff]);
        std::vector<real> z_segment = interpolation(x_l, 1/z_left[diff], x_r, 1/z_right[diff]);

        for(uint x = x_l; x <= x_r; ++x) {
            real intens = h_segment[x - x_l];
            minwin::Color c;
            c.red = red * intens;
            c.green = green * intens;
            c.blue = blue * intens;

            real overZ = z_segment[x_r-x];
            if(overZ > depth_buffer[x][y]){
                windows.put_pixel(x, y, c);
                depth_buffer[x][y] = overZ;
            }
        }
    }
}

void Scene::draw_filled_triangle(const Vec3r &v1, const Vec3r &v2, const Vec3r &v3) const {
    Vec3r p0 = Vec3r(v1);
    Vec3r p1 = Vec3r(v2);
    Vec3r p2 = Vec3r(v3);
    
    if(p1[1] < p0[1])
        std::swap(p1, p0);
    if(p2[1] < p0[1])
        std::swap(p2, p0);
    if(p2[1] < p1[1])
        std::swap(p2, p1);

    int x0 = std::round(p0[0]), x1 = std::round(p1[0]), x2 = std::round(p2[0]);
    int y0 = std::round(p0[1]), y1 = std::round(p1[1]), y2 = std::round(p2[1]);
    real z0 = p0[2], z1 = p1[2], z2 = p2[2];

    std::vector<real> x02 = interpolation(y0, x0, y2, x2);
    std::vector<real> x01 = interpolation(y0, x0, y1, x1);
    std::vector<real> x12 = interpolation(y1, x1, y2, x2);

    std::vector<real> z02 = interpolation(y0, z0, y2, z2);
    std::vector<real> z01 = interpolation(y0, z0, y1, z1);
    std::vector<real> z12 = interpolation(y1, z1, y2, z2);
    
    x01.pop_back();
    x01.insert(x01.end(), x12.begin(), x12.end());

    z01.pop_back();
    z01.insert(z01.end(), z12.begin(), z12.end());

    std::vector<real> x_left, x_right;
    std::vector<real> z_left, z_right;
    int m = (int)(std::floor(x01.size()/2));
    if(x02[m] < x01[m]) {
        x_left = x02;
        x_right = x01;

        z_left = z02;
        z_right = z01;
    }
    else {
        x_left = x01;
        x_right = x02;

        z_left = z01;
        z_right = z02;
    }
    
    for(int y = y0; y <= y2; ++y) {
        int diff = y - y0;
        uint x_l = std::round(x_left[diff]);
        uint x_r = std::round(x_right[diff]);

        std::vector<real> z_segment = interpolation(x_l, 1/z_left[diff], x_r, 1/z_right[diff]);

        for(uint x = x_l; x <= x_r; ++x) {
            real overZ = z_segment[x_r-x];
            if(overZ > depth_buffer[x][y]){
                windows.put_pixel(x, y);
                depth_buffer[x][y] = overZ;
            }
        }
    }
}

real sign(const int value) {
    if(value < 0)
        return -1;
    return 1;
}

std::vector<real> Scene::interpolation(int i0, real h0, int i1, real h1) const {
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

void Scene::draw_line(const Vec3r &v1, const Vec3r &v2) const {
    int x0 = std::round(v1[0]), y0 = std::round(v1[1]), x1 = std::round(v2[0]), y1 = std::round(v2[1]);

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

void Scene::shutdown() {
    for(int i=0; i<600; ++i)
        delete[] depth_buffer[i];
    delete[] depth_buffer;
    windows.close(); 
}


QuitButtonBehavior::QuitButtonBehavior( Scene & scene ) : owner(scene) {}
void QuitButtonBehavior::on_click() const { owner.quit(); }


ChangeModeBehavior::ChangeModeBehavior( Scene & scene ) : owner(scene) {}
void ChangeModeBehavior::on_press() const { owner.changeMode(); }
void ChangeModeBehavior::on_release() const {  }


ForwardXBehavior::ForwardXBehavior( Scene & scene ) : owner(scene) {}
void ForwardXBehavior::on_press() const { owner.getCamera().move(0, Forward::X); }
void ForwardXBehavior::on_release() const { owner.getCamera().stopMove(0); }


BackwardXBehavior::BackwardXBehavior( Scene & scene ) : owner(scene) {}
void BackwardXBehavior::on_press() const { owner.getCamera().move(0, Backward::X); }
void BackwardXBehavior::on_release() const { owner.getCamera().stopMove(0); }


ForwardZBehavior::ForwardZBehavior( Scene & scene ) : owner(scene) {}
void ForwardZBehavior::on_press() const { owner.getCamera().move(2, Forward::Z); }
void ForwardZBehavior::on_release() const { owner.getCamera().stopMove(2); }


BackwardZBehavior::BackwardZBehavior( Scene & scene ) : owner(scene) {}
void BackwardZBehavior::on_press() const { owner.getCamera().move(2, Backward::Z); }
void BackwardZBehavior::on_release() const { owner.getCamera().stopMove(2); }


ForwardYBehavior::ForwardYBehavior( Scene & scene ) : owner(scene) {}
void ForwardYBehavior::on_press() const { owner.getCamera().move(1, Forward::Y); }
void ForwardYBehavior::on_release() const { owner.getCamera().stopMove(1); }


BackwardYBehavior::BackwardYBehavior( Scene & scene ) : owner(scene) {}
void BackwardYBehavior::on_press() const { owner.getCamera().move(1, Backward::Y); }
void BackwardYBehavior::on_release() const { owner.getCamera().stopMove(1); }


CwXBehavior::CwXBehavior( Scene & scene ) : owner(scene) {}
void CwXBehavior::on_press() const { owner.getCamera().rotate(0, Clockwise::X); }
void CwXBehavior::on_release() const { owner.getCamera().stopRotate(0); }


AcwXBehavior::AcwXBehavior( Scene & scene ) : owner(scene) {}
void AcwXBehavior::on_press() const { owner.getCamera().rotate(0, AntiClockwise::X); }
void AcwXBehavior::on_release() const { owner.getCamera().stopRotate(0); }


CwYBehavior::CwYBehavior( Scene & scene ) : owner(scene) {}
void CwYBehavior::on_press() const { owner.getCamera().rotate(1, Clockwise::Y); }
void CwYBehavior::on_release() const { owner.getCamera().stopRotate(1); }


AcwYBehavior::AcwYBehavior( Scene & scene ) : owner(scene) {}
void AcwYBehavior::on_press() const { owner.getCamera().rotate(1, AntiClockwise::Y); }
void AcwYBehavior::on_release() const { owner.getCamera().stopRotate(1); }


CwZBehavior::CwZBehavior( Scene & scene ) : owner(scene) {}
void CwZBehavior::on_press() const { owner.getCamera().rotate(2, Clockwise::Z); }
void CwZBehavior::on_release() const { owner.getCamera().stopRotate(2); }


AcwZBehavior::AcwZBehavior( Scene & scene ) : owner(scene) {}
void AcwZBehavior::on_press() const { owner.getCamera().rotate(2, AntiClockwise::Z); }
void AcwZBehavior::on_release() const { owner.getCamera().stopRotate(2); }