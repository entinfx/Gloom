#ifndef Sphere_hpp
#define Sphere_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "../Core/Material.hpp"
#include "../Hitable/Hitable.hpp"

class Material;

class Sphere: public Hitable {
    Vector3 center;
    double radius;
public:
    Sphere() {};
    Sphere(Vector3 center, double radius, Material *material): center(center),
                                                              radius(radius),
                                                              material(material) {};
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRecord &hitRecord) const;
    Material *material;
};

/* Ray-Sphere intersection */
// * Sphere equation: X^2 + Y^2 + Z^2 = R^2, any point X,Y,Z
//   that satisfies the equation is on the sphere.
//   Sphere equation for center C(cX,cY,cZ):
//   (X-cX)^2 + (Y-cY)^2 + (Z-cZ)^2 = R^2.
// * Vector from center point C to any point p(t) on the ray
//   is (p - C).
//   If the ray hits the sphere, there will be 2 vectors (p - C)
//   that match the radius to p(t) vector, so their length is equal:
//   ||(p - C)|| = r, same as:
//   sqrt((p - C) • (p - C)) = r, same as:
//   (p - C) • (p - C) = r * r.
//   Any point p that satisfies the equation will intersect with
//   the sphere.
//   p(t) = A + t * B, so:
//   (A + t * B - C) • (A + t * B - C) = r * r.
//   Rearrange it and solve a quadratic equation for t1 and t2:
//   tt * (B • B) + 2t * (B • (A - C)) + ((A - C) • (A - C)) - RR = 0
inline bool Sphere::hit(const Ray &ray, double tMin, double tMax, HitRecord &hitRecord) const {
    Vector3 oc = ray.origin() - center;
    double a = dot(ray.direction(), ray.direction());
    double b = dot(oc, ray.direction()); // b is divided by 2
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        // Outer surface hit t
        double t = (-b - sqrt(discriminant)) / a;
        if (t < tMax && t > tMin) {
            hitRecord.t = t;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - center) / radius;
            hitRecord.material = this->material;
            return true;
        }
        /* Inner surface hit t */
        // When ray hits the outer surface, if BSDF sends a new ray
        // inside the Sphere it originates at the first intersection
        // (outer surface), so its origin t = 0 is less than tMin
        // (0.001) so the first intersection check fails, and this
        // t is calculated.
        t = (-b + sqrt(discriminant)) / a;
        if (t < tMax && t > tMin) {
            hitRecord.t = t;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - center) / radius;
            hitRecord.material = this->material;
            return true;
        }
    }
    return false;
}

#endif
