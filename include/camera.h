#pragma once
#include "vector.h"
#include "matrix.h"
#include "frustum.h"

namespace aline {

class Camera {
public:
    real aspectRatio;
    real focalDistance;
    Vec3r position;
    Vec3r rotation;
    real speedMove;
    real speedRotation;
    real speedZoom;
    Frustum fov;

    int cptMove;
    int cptRotate;

    Vec3r moveAdd;
    Vec3r rotateAdd;

    bool moveTab[3];

    Camera(real aspectRatio, real focalDistance = 2, Vec3r position = {}, Vec3r rotation = {}, real speedMove = 0.125, real speedRotation = 0.25, real speedZoom = 0.0625);

    Mat44r tranform() const;

    void move(int idAxis, const Vec3r &axis);
    void rotate(const Vec3r &axis);

    void stopMove(int axis);
    void stopRotate();

    void update();
};

}