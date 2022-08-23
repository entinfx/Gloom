#ifndef Glossy_hpp
#define Glossy_hpp

#include <iostream>
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

class Glossy: public Material {
    Vector3d albedo;
public:
    Glossy(const Vector3d &albedo): albedo(albedo) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const;
};

inline bool Glossy::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const {
    double cosine = 1.5 * dot(rayIn.direction(), hitRecord.normal) / rayIn.direction().length();
    double fresnelFactor = schlick(-cosine, 1.5); // reflection probability
    if (drand48() < fresnelFactor) {
        // Specular
        Vector3d reflected = reflect(unitVector(rayIn.direction()), hitRecord.normal);
        scattered = Ray(hitRecord.p, reflected);
        attenuation = Vector3d(1, 1, 1);
        return (dot(scattered.direction(), hitRecord.normal) > 0); // return true only for rays coming outwards (some rays don't)
    } else {
        // Diffuse
        Vector3d target = hitRecord.p + hitRecord.normal + randomInUnitSphere();
        scattered = Ray(hitRecord.p, target - hitRecord.p);
        attenuation = albedo;
        return true;
    }
}


#endif
