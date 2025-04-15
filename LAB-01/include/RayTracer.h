#ifndef _RayTracer_H_
#define _RayTracer_H_

#include "Scene.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"

class RayTracer {
private: 
	Scene * scene;
	Image * image;
    Vec3f bgColor;

public:
    RayTracer(const int w, const int h) {
        this->scene = new Scene();
        this->image = new Image(w, h);
        this->bgColor = Vec3f(0.69f, 0.69f, 0.69f);
    }
    ~RayTracer() {
        delete scene;
        delete image;
    }
    void searchClosestHit(const Ray & ray, HitRec & hitRec);
	Vec3f getEyeRayDirection(int x, int y);
    void fireRays();
    void toPPM(const char* path) {
        this->image->toPPM(path);
    };
    void toBMP(const char* path) {
        this->image->toBMP(path);
    };
    void addSphere(const Vec3f & c, const float r, const Material* mat) {
        if (r >= 0.0f) {
            this->scene->addSphere(c, r, mat);
        }
    };

	void addLight(const Vec3f& pos, const Vec3f& ambient, const Vec3f& diffuse, Vec3f& specular) {
		this->scene->addLight(pos, ambient, diffuse, specular);
	};

	Vec3f CalculateColor(const Ray & ray, const HitRec & hitrec, const Light* light);

	void addPlane(const Vec3f& p, const Vec3f& n, const Material* mat) {
        this->scene->addPlane(p, n, mat);
	};
};

#endif