#ifndef Dielectric_hpp
#define Dielectric_hpp

#include <iostream>
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

class Dielectric: public Material {
    Vector3d attenuation;
    double refractionIndex;
public:
    Dielectric(Vector3d a, double ri): attenuation(a), refractionIndex(ri) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const;
};

inline bool Dielectric::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const {
    Vector3d outwardNormal;
    Vector3d reflected = reflect(rayIn.direction(), hitRecord.normal);
    double niOverNt;
    attenuation = this->attenuation;
    Vector3d refracted;
    double fresnelFactor; // reflection probability
    double cosine;
    // Check if ray is inside our outside the sphere
    if (dot(rayIn.direction(), hitRecord.normal) > 0) {
        outwardNormal = -hitRecord.normal;
        niOverNt = refractionIndex;
        cosine = refractionIndex * dot(rayIn.direction(), hitRecord.normal) / rayIn.direction().length(); // ???: Why multiply by IOR?
    } else {
        outwardNormal = hitRecord.normal;
        niOverNt = 1.0 / refractionIndex;
        cosine = -dot(rayIn.direction(), hitRecord.normal) / rayIn.direction().length();
    }
    // Get Fresnel factor (Schlick approximation)
    if (refract(rayIn.direction(), outwardNormal, niOverNt, refracted)) {
        fresnelFactor = schlick(cosine, refractionIndex);
    } else {
        fresnelFactor = 1.0;
    }
    // Reflect or refract based on Fresnel factor
    if (drand48() < fresnelFactor) {
        scattered = Ray(hitRecord.p, reflected);
    } else {
        scattered = Ray(hitRecord.p, refracted + 0.005 * randomInUnitSphere());
    }
    return true;
}

#endif
