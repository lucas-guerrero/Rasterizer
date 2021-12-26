#include <cmath>
#include "frustum.h"

using namespace aline;

Frustum::Frustum(real dNear, real dFar, real a, real e):dNear(dNear), dFar(dFar)  {
    near = {0, 0, -1, -dNear};
    far = {0, 0, 1, dFar};
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}

void Frustum::update(real a, real e) {
    near = {0, 0, -1, -dNear};
    far = {0, 0, 1, dFar};
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}

Vec4r Frustum::interseption(const Vec4r &point, const Vec4r &direction, const Vec4r &plan) {
    real iV = dot(plan, direction);
    real iS = dot(plan, point);

    real t = -(iS/iV);

    return point + t * direction;
}

Object Frustum::clippingCut(const Object &object, const Vec4r &plan) {
    std::vector<Face> newFaces;
    std::vector<Vertex> newVertices(object.get_vertices());

    for(const auto face: object.get_faces()) {
        Vertex v1 = newVertices[face.idP1];
        Vertex v2 = newVertices[face.idP2];
        Vertex v3 = newVertices[face.idP3];

        Vec3r p1 = v1.point;
        Vec3r p2 = v2.point;
        Vec3r p3 = v3.point;

        real distP1 = dot(p1, {plan[0], plan[1], plan[2]}) + plan[3];
        real distP2 = dot(p2, {plan[0], plan[1], plan[2]}) + plan[3];
        real distP3 = dot(p3, {plan[0], plan[1], plan[2]}) + plan[3];

        int isIn[3] = {0, 0, 0};

        if(distP1 >= 0) isIn[0] = 1;
        if(distP2 >= 0) isIn[1] = 1;
        if(distP3 >= 0) isIn[2] = 1;

        int sum = isIn[0] + isIn[1] + isIn[2];

        if(sum == 3) newFaces.push_back(face);
        else if(sum == 0) continue;
        else {
            Vec3r a = p1;
            Vec3r b = p2;
            Vec3r c = p3;

            if(sum == 1) {
                if(isIn[0] == 1) {
                    Vec4r prime1 = interseption({a[0], a[1], a[2], 1}, {b[0], b[1], b[2], 0}, plan);
                    Vec4r prime2 = interseption({a[0], a[1], a[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v2.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.idP1, size-2, size-1, face.color));
                }

                else if(isIn[1] == 1) {
                    Vec4r prime1 = interseption({b[0], b[1], b[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = interseption({b[0], b[1], b[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(size-2, face.idP2, size-1, face.color));
                }

                else {
                    Vec4r prime1 = interseption({c[0], c[1], c[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = interseption({c[0], c[1], c[2], 1}, {b[0], b[1], b[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v2.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(size-2, size-1, face.idP3, face.color));
                }
            }

            else {
                if(isIn[0] == 0) {
                    Vec4r prime1 = interseption({b[0], b[1], b[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = interseption({c[0], c[1], c[2], 1}, {a[0], a[1], a[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v1.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.idP2, face.idP3, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.idP3, size-1, face.color));
                }

                else if(isIn[1] == 0) {
                    Vec4r prime1 = interseption({a[0], a[1], a[2], 1}, {b[0], b[1], b[2], 0}, plan);
                    Vec4r prime2 = interseption({c[0], c[1], c[2], 1}, {b[0], b[1], b[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v2.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v2.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.idP1, face.idP3, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.idP3, size-1, face.color));
                }

                else {
                    Vec4r prime1 = interseption({a[0], a[1], a[2], 1}, {c[0], c[1], c[2], 0}, plan);
                    Vec4r prime2 = interseption({b[0], b[1], b[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v3.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.idP1, face.idP2, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.idP2, size-1, face.color));
                }
            }
        }
    }


    Shape s(object.shape.get_name(), newVertices, newFaces);

    return Object(s);
}

Object Frustum::clipping(const Object& object) {
    //real rayon = 10;
    real rayon = object.rayon;
    Vec3r centre = object.barycentre;

    real distNear = dot(centre, {near[0], near[1], near[2]}) + near[3];
    real distFar = dot(centre, {far[0], far[1], far[2]}) + far[3];
    real distLeft = dot(centre, {left[0], left[1], left[2]}) + left[3];
    real distRight = dot(centre, {right[0], right[1], right[2]}) + right[3];
    real distBot = dot(centre, {bottom[0], bottom[1], bottom[2]}) + bottom[3];
    real distTop = dot(centre, {top[0], top[1], top[2]}) + top[3];

    // Verification de l'objet en dehors du frustum

    if(distNear < -rayon) return Object(Shape("", {}, {}));
    if(distFar < -rayon) return Object(Shape("", {}, {}));
    if(distLeft < -rayon) return Object(Shape("", {}, {}));
    if(distRight < -rayon) return Object(Shape("", {}, {}));
    if(distTop < -rayon) return Object(Shape("", {}, {}));
    if(distBot < -rayon) return Object(Shape("", {}, {}));

    Object o(object);

    if(distNear < rayon) o = clippingCut(o, near);
    if(distFar < rayon) o = clippingCut(o, far);
    if(distLeft < rayon) o = clippingCut(o, left);
    if(distRight < rayon) o = clippingCut(o, right);
    if(distTop < rayon) o = clippingCut(o, top);
    if(distBot < rayon) o = clippingCut(o, bottom);

    return o;
}