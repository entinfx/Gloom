#ifndef Glossy_hpp
#define Glossy_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "../Core/Material.hpp"

class Glossy: public Material {
    Vector3 albedo;
public:
    Glossy(const Vector3 &albedo): albedo(albedo) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const;
};

inline bool Glossy::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const {
    double cosine = 1.5 * dot(rayIn.direction(), hitRecord.normal) / rayIn.direction().length();
    double fresnelFactor = schlick(-cosine, 1.5); // reflection probability
    if (drand48() < fresnelFactor) {
        // Specular
        Vector3 reflected = reflect(unitVector(rayIn.direction()), hitRecord.normal);
        scattered = Ray(hitRecord.p, reflected);
        attenuation = Vector3(1, 1, 1);
        return (dot(scattered.direction(), hitRecord.normal) > 0); // return true only for rays coming outwards (some rays don't)
    } else {
        // Diffuse
        Vector3 target = hitRecord.p + hitRecord.normal + randomInUnitSphere();
        scattered = Ray(hitRecord.p, target - hitRecord.p);
        attenuation = albedo;
        return true;
    }
}


#endif
