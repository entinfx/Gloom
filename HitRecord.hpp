#ifndef HitRecord_hpp
#define HitRecord_hpp

#include <iostream>
#include "Vector3d.hpp"

// Forward declare class Material so the struct
// HitRecord knows about its existence and allows
// a pointer to it as a field.
// A non-pointer field however would require
// including the entire class so that
// sizeof(material) could be calculated.
//
// Forward declaration is necessary because otherwise
// there is a circular dependency where HitRecord.hpp
// includes Material.hpp and Material.hpp includes
// HitRecord.hpp
class Material;

struct HitRecord {
    double t;
    Vector3d p;
    Vector3d normal;
    Material *material;
};

#endif
