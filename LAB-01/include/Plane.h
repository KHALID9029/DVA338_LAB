#ifndef _PLANE_H_
#define _PLANE_H_

#include "Object.h"
#include "Vec3.h"
#include "Material.h"
#include "Ray.h"

class Plane : public Object {

public:
	Vec3f point;
	Vec3f normal;
	const Material* material;

	Plane(const Vec3f& p, const Vec3f& n, const Material* mat) 
		: point(p), normal(n), material(mat) {
		normal=normal.normalize();
	}

	bool hit(const Ray& r, HitRec& rec) const {
		float denom = normal.dot(r.d);

		// We can't divide by zero, so we check if the ray is parallel to the plane
		if (fabs(denom) > 1e-6) {

			//n*p = -D
			float D = -(normal.dot(point));
			//t = -(n*o + D) / (n*d)
			float t = -(normal.dot(r.o) + D) / denom;

			if (t >=0 && t < rec.tHit && t<r.tClip) {
				rec.tHit = t;
				rec.anyHit = true;
				rec.p = r.o + r.d * t;
				rec.n = normal;
				rec.material = material;
				return true;
			}
		}
		return false;
	}

};
#endif // !_PLANE_H_