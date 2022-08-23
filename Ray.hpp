#ifndef Ray_hpp
#define Ray_hpp

#include <iostream>
#include "Vector3d.hpp"

// * Ray is a vector with an origin point:
//   p(t) = A + t * B, where
//   p - point along the Ray
//   a - origin point
//   b - vector
//   t - double constant
//
// * Ray in 3d space:
//        0t        1t        2t
//   ------*--------->---------------->
//         a     b
class Ray {
    Vector3d a;
    Vector3d b;
public:
    Ray() {};
    Ray(const Vector3d &a, const Vector3d &b): a(a), b(b) {};
    Vector3d origin() const;
    Vector3d direction() const;
    Vector3d pointAtParameter(double t) const;
};

inline Vector3d Ray::origin() const { return this->a; }
inline Vector3d Ray::direction() const { return this->b; }
inline Vector3d Ray::pointAtParameter(double t) const { return this->a + t * this->b; }

#endif
