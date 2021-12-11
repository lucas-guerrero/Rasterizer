#include "camera.h"
#include "quaternion.h"

using namespace aline;

Camera::Camera(real aspectRatio, real focalDistance, Vec3r position, Vec3r rotation, real speedMove, real speedRotation, real speedZoom):
    aspectRatio(aspectRatio), focalDistance(focalDistance),
    position(position), rotation(rotation),
    speedMove(speedMove), speedRotation(speedRotation), speedZoom(speedZoom),
    fov(focalDistance, 50, aspectRatio, focalDistance) { }


void Camera::move(int idAxis, const Vec3r &axis) {
    bool isAxisActive = moveTab[idAxis];
    if(!isAxisActive) {
        moveAdd += axis*speedMove;
        moveTab[idAxis] = true;
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
    moveTab[idAxis] = false;
    moveAdd[idAxis] = 0;
}

void Camera::stopRotate(int idAxis) {
    rotateTab[idAxis] = false;
    rotateAdd[idAxis] = 0;
}

void Camera::update() {
    position += moveAdd;
    rotation += rotateAdd;
}

Mat44r Camera::transform() const {

    Mat44r matrixR = matrixRotationQuaternion();

    Mat44r matrixTranslation = {
        {1, 0, 0, -position[0]},
        {0, 1, 0, -position[1]},
        {0, 0, 1, -position[2]},
        {0, 0, 0, 1}
    };

    return matrixR * matrixTranslation;
}

Mat44r Camera::matrixRotationQuaternion() const{
    real deltaX = (rotation[0]*PI /180)/2;
    real deltaY = (rotation[1]*PI /180)/2;
    real deltaZ = (rotation[2]*PI /180)/2;

    Quaternion qX(std::cos(deltaX), {std::sin(deltaX), 0, 0});
    Quaternion qY(std::cos(deltaY), {0, std::sin(deltaY), 0});
    Quaternion qZ(std::cos(deltaZ), {0, 0, std::sin(deltaZ)});

    Quaternion qRotation = qZ.mult(qY);

    qRotation = qRotation.mult(qX);

    Quaternion q(qRotation.s, qRotation.v);
    q.v = -q.v;

    return q.transformToMatrix();
}
