#ifndef HitableList_hpp
#define HitableList_hpp

#include <iostream>
#include "../Core/Ray.hpp"
#include "../Core/HitRecord.hpp"
#include "Hitable.hpp"

class HitableList: public Hitable {
    Hitable **list;
    int listSize;
public:
    HitableList() {};
    HitableList(Hitable **l, int n): list(l), listSize(n) {};
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const;
};

inline bool HitableList::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const {
    HitRecord tempHitRecord;
    bool hitAnything = false;
    double closestSoFar = tMax;
    /* Find the closest hit */
    for (int i = 0; i < listSize; i++) {
        if (list[i]->hit(ray, tMin, closestSoFar, tempHitRecord)) {
            // If current object in list is hit and is the closest
            // one so far save its HitRecord and shrink the t range
            // by updating the t value.
            hitAnything = true;
            closestSoFar = tempHitRecord.t;
            hitRecord = tempHitRecord;
        }
    }
    return hitAnything;
}

#endif
