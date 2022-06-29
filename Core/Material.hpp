#ifndef Material_hpp
#define Material_hpp

#include <iostream>
#include "Vector3.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"

// Any Material should have the scatter() function that
// saves attenuation and scattered ray based on input ray
// (rayIn) and hitRecord (with hit point, normal, ray length
// (t)). Returns true if ray was scattered.
class Material {
public:
    // Pure virtual member function
    virtual bool scatter(const Ray &rayIn, const HitRecord &hitRecord, Vector3 &attenuation, Ray &scattered) const = 0;
    virtual Vector3 emitted() const;
    // The following functions will be called on const *this in
    // derived classes so they have to be either friends
    // or const members.
    friend Vector3 randomInUnitSphere();
    friend Vector3 reflect(const Vector3 &v, const Vector3 &n);
    friend float schlick(float cosine, float refractionIndex);
    friend bool refract(const Vector3 &v, const Vector3 &n, float niOverNt, Vector3 &refracted);
};

/* Emitted radiance */
// This function must be overriden by light emitters.
// Because this function is virtual (not pure virtual)
// Materials that don't override it will have 0 emittance.
inline Vector3 Material::emitted() const {
    return Vector3(0, 0, 0);
}

/* Diffuse reflection */
// Scattered light direction is random.
inline Vector3 randomInUnitSphere() {
    Vector3 p;
    do {
         p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1.0, 1.0, 1.0);
    } while (p.squaredLength() >= 1.0);
    return p;
}

/* Specular reflection */
// Mirror-like reflection. Angle of the incidence equals
// the angle of reflection.
//            ^N
//         A  |   A
//       *---->---->
//        \   |   /
//        I\  |B / R
//          \ | /
//           \|/
// -----------*-----------
// I - incident ray, R - reflected ray, N - normal
// B = N * (N • I) // N • I < 0
// A = I - B
// R = A - B =
//     I - 2 * B =
//     I - 2 * (N • I) * N
inline Vector3 reflect(const Vector3 &v, const Vector3 &n) {
    return v - 2 * dot(v, n) * n;
}

/* Schlick approximation of Fresnel effect */
// * Christophe Schlick's approximation of the contribution
//   of Fresnel factor in the specular reflection from a
//   non-conducting (dielectric) surface between 2 media.
// * Fresnel equations describe the reflection and the
//   transmission (refraction) of light when incident on an
//   interface between 2 different optical media.
//   Fresnel effect is most noticeable in non-conductors
//   and less so in conductors. However conductors also
//   experience fresnel effect and have an IOR that is a
//   complex number with an imaginary part.
//
// * Parameters:
//   cosine - cos of incident light angle
//   refractionIndex - IOR of slow medium
inline float schlick(float cosine, float refractionIndex) {
    // Specular reflection coefficient of light incoming parallel
    // to normal (minimal reflection)
    float r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 *= r0;
    // Specular reflection coefficient
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

/* Refraction */
// * Snell's law: v1/sin(Ø1) = v2/sin(Ø2),
//   Index of Refraction (IOR). For 2 media:
//   n1 = c / v1,
//   n2 = c / v2, so:
//   n1 * sin(Ø1) = n2 * sin(Ø2), or:
//   sin(Ø2) / sin(Ø1) = n1 / n2
//
//              |N
//         *----*
//         I\   |C
//           \Ø1|
//            \ |
//             \| A
//   *----------*---->----->M
//   -1         |\   |     1
//              | \T |
//              |Ø2\ |B
//              |   \|
//              *----*
//              |-N
//
//   Vectors:
//   I - incident ray direction
//   T - refracted ray direction
//   C = (I • C) * (-N) = N * cos(Ø1)
//   M = (I + C) / sin(Ø1) = M * sin(Ø2)
//   A = (T • M) * M = M * sin(Ø2)
//   B = ((-N) • T) * (-N) = -N * cos(Ø2)
//
//   Scalars:
//   C1 = N • (-I) = cos(Ø1)
//   C2 = (-N) • (T) = cos(Ø2)
//
// * Calculate T:
//   T = A + B,
//   T = M * sin(Ø2) - N * cos(Ø2),
//
//       (I + C) * sin(Ø2)
//   T = ----------------- - N * cos(Ø2),
//            sin(Ø1)
//
//       (I + N * cos(Ø1)) * sin(Ø2)
//   T = ------------------------- - N * cos(Ø2).
//                sin(Ø1)
//
//   sin(Ø2)  n1
//   ------ = --, so:
//   sin(Ø1)  n2
//
//       n1
//   T = -- * (I + N * cos(Ø1)) - N * cos(Ø2).
//       n2
//
//   cos(Ø) = sqrt(1 - sin^2(Ø)),
//
//             n1
//   sin(Ø2) = -- * sin(Ø1), so:
//             n2
//
//       n1                                    n1^2
//   T = -- * (I + N * cos(Ø1)) - N * sqrt(1 - ---- * sin^2(Ø1)).
//       n2                                    n2^2
//
//   n = n1 / n2,
//   C1 = N • (-I) = cos(Ø1),
//                                      n1^2
//   C2 = (-N) • T = cos(Ø2) = sqrt(1 - ---- * sin^2(Ø1)).
//                                      n2^2
//
//   T = n * (I + N * C1) - N * C2 =
//       n * I + N * (n * C1 - C2).
//
// * Total Internal Reflection (TIR) */
//   For rays travelling from the slow to fast medium (glass -> air/vacuum)
//   there is an angle Ø1 for which the refracted ray angle Ø2 = 90 deg.
//   So for any angle >= Ø1 we experience a total internal reflection
//   as the refraction ray is refracted back inside the slow medium.
//   This happens when the term within the sqrt of C2 is negative.
//
//              ^N
//              |
//              |
//   FAST       |
//              |Ø2
//   -----------*----------> Refracted ray >= 90 deg.
//             /
//   SLOW     /Ø1
//           / Incident ray >= Ø1 (travels towards surface)
//          /
inline bool refract(const Vector3 &v, const Vector3 &n, float niOverNt, Vector3 &refracted) {
    Vector3 uv = unitVector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
    // Check for Total Internal Reflection
    if (discriminant > 0) {
        // This function only receives corrected normals relative
        // to their incident rays. That means that dot(uv, n) is
        // always the same sign (negative in current implementation).
        // There are 2 ways to correct for that:
        // 1. Use original T equation: T = n * I + N * (n * C1 - C2)
        //    and make C1 (dt or dot(uv, n)) always positive.
        // 2. Flip signs in the original T equation
        //    T = n * I - N * (n * C1 + C2)
        
        // Option #1
        // refracted = niOverNt * uv + n * (niOverNt * abs(dt) - sqrt(discriminant));
        
        // Option #2
        // refracted = niOverNt * uv - n * (niOverNt * dt + sqrt(discriminant));, or:
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        
        // No corrections (incorrect)
        // refracted = niOverNt * uv + n * (niOverNt * dt - sqrt(discriminant));
        
        return true;
    } else {
        // TIR: No refraction
        return false;
    }
}

#endif
