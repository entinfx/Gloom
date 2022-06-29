#ifndef DiffuseLight_hpp
#define DiffuseLight_hpp

#include <iostream>
#include "../Core/Vector3.hpp"
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "../Core/Material.hpp"

class DiffuseLight: public Material {
    Vector3 color;
public:
    DiffuseLight(Vector3 color): color(color) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const;
    virtual Vector3 emitted() const;
};

inline bool DiffuseLight::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const {
    return false;
}

inline Vector3 DiffuseLight::emitted() const {
    return color;
}

#endif
