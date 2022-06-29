#ifndef Camera_hpp
#define Camera_hpp

#include <iostream>
#include "Vector3.hpp"
#include "Ray.hpp"

class Camera {
    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u;
    Vector3 v;
    Vector3 w;
    float lensRadius;
public:
    Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 vUp, float vFov, float aspect, float aperture, float focusDistance);
    inline Ray getRay(float s, float t, Vector3 randomOffset) const;
    friend Vector3 randomInUnitDisk();
};

inline Vector3 randomInUnitDisk() {
    Vector3 p;
    do {
        p = 2 * Vector3(drand48(), drand48(), 0) - Vector3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

/* Reverse Pinhole Camera */
//
//    h    h
//  *----*----* Image plane
//   \   |   /
//    \  |d /
//     \ | /
//      \Ø/
// ------*------ Reverse pinhole
//
// Ø - FOV angle
// d - distance from pinhole to image plane (film)
// h - half of image plane height (h = tan(Ø/2) * d)
// ƒ - focal length (ƒ = ∞)
//
// Let's assume the distance from the pinhole to the
// image plane is equal to 1 (d = 1).
// We then can adjust the FOV angle to make the image
// plane smaller (narrower Ø) or bigger (wider Ø):
// h = tan(Ø/2) * 1
//
// Let's assume h = 1, then the image height is equal
// to 2.
// Let's assume the aspect ratio is equal to 2, then
// the image width would be equal to 4.
//
//              ^ v * (0,2,0)
//    (-2,1,-1) |
//    *---------|--nx-----------*(2, 1, -1)
//   /|         |              /|
//  / (-2,1,0)  |      (2,1,0)/ |
// Y------------+------------*  ny
// |  |         |            |  |
// |  |         |(0,0,0)     |  |
// |  |         *------------+--|--------------> u * (4,0,0)
// |  Z----------------------|--*(2,-1,-1)
// | /(2,-1,-1)              | /
// |/                        |/
// *-------------------------X
// (-2,-1,0)         (2,-1,0)
//
// (-2,1,-1)           (2,1,-1)
// Y--------------------------*
// |                          |
// |        U                 |
// |----------------*         |
// |                |         |
// |                |V        |
// |                |         |
// |                |         |
// *--------------------------X
// (-2,-1,-1)          (2,-1,-1)
//
// Calculate guiding vectors.
//
// We must find a vector that after addition
// of lower_left vector (-2,-1,-1) will point
// at top right corner of the screen (2,1,-1).
//
// lower_left + (x,y,z) = top_right
// (-2,-1,-1) + (x,y,z) = 2,1,-1
//              (x,y,z) = 4,2,0
//
// Find a vector that will point at lower left
// corner of the screen (-2,-1,-1).
//
// lower_left + (x,y,z) = bottom_left
// (-2,-1,-1) + (x,y,z) = -2,-1,-1
//              (x,y,z) = 0,0,0
//
// Any vector in range (0,0,0)...(4,2,0) + lower_left (-2,-1,-1)
// will point at some point on the screen (-2,-1,-1)...(2,1,-1).
// (4,2,0) = 1 * (4,0,0) + 1 * (0,2,0)
// (0,0,0) = 0 * (4,0,0) + 0 * (0,2,0)
// If u and v are variables in range (0...1), then:
// u * (4,0,0) + v * (0,2,0) = (-2,-1,-1)...(2,1,-1).
Camera::Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 vUp, float vFov, float aspect, float aperture, float focusDistance) {
    lensRadius = aperture / 2;
    float theta = vFov * M_PI / 180;
    // Assuming the distance between origin and image plane (d)
    // is equal to 1, the ratio between the vertical FOV (Ø) and
    // the half height of the image plane is:
    float halfHeight = tan(theta / 2);
    float halfWidth = aspect * halfHeight;
    origin = lookFrom;
    w = unitVector(lookFrom - lookAt);
    u = unitVector(cross(vUp, w));
    v = cross(w, u);
    lowerLeftCorner = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
    horizontal = 2 * halfWidth * focusDistance * u;
    vertical = 2 * halfHeight * focusDistance * v;
}

inline Ray Camera::getRay(float s, float t, Vector3 cameraOffset) const {
    Vector3 rd = lensRadius * cameraOffset;
    Vector3 offset = u * rd.x() + v * rd.y();
    return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
}

#endif
