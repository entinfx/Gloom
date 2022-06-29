#ifndef Ray_hpp
#define Ray_hpp

#include <iostream>
#include "Vector3.hpp"

// * Ray is a vector with an origin point:
//   p(t) = A + t * B, where
//   p - point along the Ray
//   a - origin point
//   b - vector
//   t - float constant
//
// * Ray in 3d space:
//        0t        1t        2t
//   ------*--------->---------------->
//         a     b
class Ray {
    Vector3 a;
    Vector3 b;
public:
    Ray() {};
    Ray(const Vector3 &a, const Vector3 &b): a(a), b(b) {};
    Vector3 origin() const;
    Vector3 direction() const;
    Vector3 pointAtParameter(float t) const;
};

inline Vector3 Ray::origin() const { return this->a; }
inline Vector3 Ray::direction() const { return this->b; }
inline Vector3 Ray::pointAtParameter(float t) const { return this->a + t * this->b; }

#endif
