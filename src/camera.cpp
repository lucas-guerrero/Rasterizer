#include "camera.h"

using namespace aline;

Camera::Camera(real aspectRatio, real focalDistance, Vec3r position, Vec3r rotation, real speedMove, real speedRotation, real speedZoom):
    aspectRatio(aspectRatio), focalDistance(focalDistance),
    position(position), rotation(rotation),
    speedMove(speedMove), speedRotation(speedRotation), speedZoom(speedZoom),
    fov(focalDistance, 50, aspectRatio, focalDistance) { }


void Camera::move(int idAxis, const Vec3r &axis) {
    //Vec3r speed = axis*speedMove;
    //position = position + speed;

    bool isAxisActive = moveTab[idAxis];
    if(!isAxisActive) {
        moveAdd += axis*speedMove;
        moveTab[idAxis] = true;
    }
}

void Camera::rotate(const Vec3r &axis) {
    ++cptRotate;
    rotateAdd += axis*speedRotation;
}

void Camera::stopMove(int idAxis) {
    moveTab[idAxis] = false;
    moveAdd[idAxis] = 0;
}

void Camera::stopRotate() {
    --cptRotate;
    if(cptRotate <= 0) {
        cptRotate = 0;
        rotateAdd = {};
    }
}

void Camera::update() {
    position += moveAdd;
    //rotation += rotateAdd;
}

Mat44r Camera::tranform() const {
    return Mat44r {
        {1, 0, 0, -position[0]},
        {0, 1, 0, -position[1]},
        {0, 0, 1, -position[2]},
        {0, 0, 0, 1}
    };
}
