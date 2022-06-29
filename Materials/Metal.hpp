#ifndef Metal_hpp
#define Metal_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "../Core/Material.hpp"

class Metal: public Material {
    Vector3 albedo;
    float fuzz;
public:
    Metal(const Vector3 &albedo, float f): albedo(albedo), fuzz(f) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const;
};

inline bool Metal::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const {
    Vector3 reflected = reflect(unitVector(rayIn.direction()), hitRecord.normal); // `this->reflect`, `this` is const
    scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction(), hitRecord.normal) > 0); // return true for Rays facing outwards (some Rays don't)
}

#endif
