#include "camera.h"

using namespace aline;

Camera::Camera(real aspectRatio, real focalDistance, real distanceMax, real speedMove, real speedRotation, real speedZoom):
    aspectRatio(aspectRatio), focalDistance(focalDistance), distanceMax(distanceMax),
    position({}), rotation({}),
    speedMove(speedMove), speedRotation(speedRotation), speedZoom(speedZoom),
    fov(focalDistance, distanceMax, aspectRatio, focalDistance), qRotation(1, {}),
    rotateAdd({}), moveTab({{}, {}, {}}), rotateTab{false, false, false} { }


void Camera::move(int idAxis, const Vec3r &axis) {
    Vec3r isAxisActive = moveTab[idAxis];
    if(isAxisActive == Vec3r{}) {
        moveTab[idAxis] = axis;
    }
}

void Camera::rotate(int idAxis, const Vec3r &axis) {
    bool isAxisActive = rotateTab[idAxis];
    if(!isAxisActive) {
        rotateAdd += axis*speedRotation;
        rotateTab[idAxis] = true;
    }
}

void Camera::stopMove(int idAxis) {
    moveTab[idAxis] = {};
}

void Camera::stopRotate(int idAxis) {
    rotateTab[idAxis] = false;
    rotateAdd[idAxis] = 0;
}

void Camera::update() {
    updateRotation();
    updateTranslation();
}

void Camera::updateRotation() {
    rotation += rotateAdd;
    real deltaX = (rotation[0]/2)*PI /180 ;
    real deltaY = (rotation[1]/2)*PI /180 ;
    real deltaZ = (rotation[2]/2)*PI /180 ;

    Quaternion qX(std::cos(deltaX), {std::sin(deltaX), 0, 0});
    Quaternion qY(std::cos(deltaY), {0, std::sin(deltaY), 0});
    Quaternion qZ(std::cos(deltaZ), {0, 0, std::sin(deltaZ)});

    Quaternion qR = qZ.mult(qY);
    qR = qR.mult(qX);

    qRotation = qR;
}

void Camera::updateTranslation() {
    real s = qRotation.s;
    Vec3r v = qRotation.v;

    real n = sq_norm(v);

    Vec3r depl = moveTab[0] + moveTab[1] + moveTab[2];

    real dote = dot(v, depl);

    Vec3r d = ( std::pow(s, 2) - n ) * depl + cross( (2*s*v) , depl) + 2*dote*v;

    Vec3r t = speedMove * d;

    position += t;
}

Mat44r Camera::transform() const {

    Mat44r matrixR = matrixRotationQuaternion();

    Mat44r matrixTransl = matrixTranslation();

    return matrixR * matrixTransl;
}

Mat44r Camera::matrixTranslation() const {
    return {
        {1, 0, 0, -position[0]},
        {0, 1, 0, -position[1]},
        {0, 0, 1, -position[2]},
        {0, 0, 0, 1}
    };
}

Mat44r Camera::matrixRotationQuaternion() const{
    Quaternion q(qRotation.s, qRotation.v);
    q.v = -q.v;

    return q.transformToMatrix();
}


bool Camera::sees(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    Vec3r p0 = v1.point;
    Vec3r p1 = v2.point;
    Vec3r p2 = v3.point;

    Vec3r e01 = p1 - p0;
    Vec3r e02 = p2 - p0;

    Vec3r normal =  cross(e01, e02) / norm(e01-e02);

    real d = dot(LOOK_IN, normal);

    return d >= 0;
}

Object Camera::culling(const Object &object) {
    std::vector<Vertex> vertices = object.get_vertices();

    std::vector<Face> faces;

    for(const auto face: object.get_faces()) {
        Vertex v0 = vertices[face.idP1];
        Vertex v1 = vertices[face.idP2];
        Vertex v2 = vertices[face.idP3];

        if(sees(v0, v1, v2)) {
            faces.push_back(face);
        }
    }

    Shape shape = Shape(object.shape.get_name(), vertices, faces);

    return Object(shape);
}