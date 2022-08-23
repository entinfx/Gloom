#ifndef DiffuseLight_hpp
#define DiffuseLight_hpp

#include <iostream>
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

class DiffuseLight: public Material {
    Vector3d color;
public:
    DiffuseLight(Vector3d color): color(color) {}
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const;
    virtual Vector3d emitted() const;
};

inline bool DiffuseLight::scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3d &attenuation, Ray &scattered) const {
    return false;
}

inline Vector3d DiffuseLight::emitted() const {
    return color;
}

#endif
