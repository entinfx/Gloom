#ifndef Lambertian_hpp
#define Lambertian_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "../Core/Material.hpp"

class Lambertian: public Material {
    Vector3 albedo;
public:
    Lambertian(const Vector3 &albedo): albedo(albedo) {};
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const;
};

inline bool Lambertian::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const {
    Vector3 target = hitRecord.p + hitRecord.normal + randomInUnitSphere(); // this->randomInUnitSphere, this is const
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    attenuation = albedo;
    return true; // always true since randomInUnitSphere vector always faces outwards
}

#endif
