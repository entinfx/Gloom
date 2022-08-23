#ifndef Hitable_hpp
#define Hitable_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"

/* Abstract class */
// Is a class in which a pure virtual (= 0) function
// exists. This function cannot be defined in abstract
// class, but must be implemented in derived classes.
class Hitable {
public:
    // Every Hitable must have hit function that determines
    // if the Ray (ray) hits the object inside the t range.
    // If so, the function returns true and fills out the
    // hitRecord.
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRecord &hitRecord) const = 0;
};

#endif
