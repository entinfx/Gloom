#ifndef Metal_hpp
#define Metal_hpp

#include <iostream>
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

class Metal: public Material {
    Vector3d albedo;
    double fuzz;
public:
    Metal(const Vector3d &albedo, double f): albedo(albedo), fuzz(f) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const;
};

inline bool Metal::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const {
    Vector3d reflected = reflect(unitVector(rayIn.direction()), hitRecord.normal); // `this->reflect`, `this` is const
    scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction(), hitRecord.normal) > 0); // return true for Rays facing outwards (some Rays don't)
}

#endif
