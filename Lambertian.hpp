#ifndef Lambertian_hpp
#define Lambertian_hpp

#include <iostream>
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

class Lambertian: public Material {
    Vector3d albedo;
public:
    Lambertian(const Vector3d &albedo): albedo(albedo) {};
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const;
};

inline bool Lambertian::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const {
    Vector3d target = hitRecord.p + hitRecord.normal + randomInUnitSphere(); // this->randomInUnitSphere, this is const
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    attenuation = albedo;
    return true; // always true since randomInUnitSphere vector always faces outwards
}

#endif
