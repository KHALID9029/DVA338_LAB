#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Ray.h"

class Object {
    private:
public:
    virtual ~Object() {};
    virtual bool hit(const Ray & r, HitRec & rec) const = 0;

};

#endif