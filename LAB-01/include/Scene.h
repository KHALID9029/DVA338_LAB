#ifndef _SCENE_H_
#define _SCENE_H_
#include <vector>
using namespace std;

#include "Sphere.h"
#include "Light.h"
#include "Plane.h"

class Scene {
private:

public:
	vector<Object*> objects;
	vector<Light*> lights;

	Scene(void) {}
	~Scene() {
		clear();
	}
	void addSphere(const Vec3f & c, const float r, const Material* mat) {
		//Store sphere as an Object pointer
        Sphere* s = new Sphere(c, r, mat);
		objects.push_back(s);
	}

	void addLight(const Vec3f& pos, const Vec3f& ambient, const Vec3f& diffuse, Vec3f& specular) {
		//Store light as an Object pointer
		Light* l = new Light(pos, ambient, diffuse, specular);
		lights.push_back(l);
	}

	void addPlane(const Vec3f& p, const Vec3f& n, const Material* mat) {
		//Store plane as an Object pointer
		Plane* pl = new Plane(p, n, mat);
		objects.push_back(pl);
	}

	void clear()
	{
		// clear objects from memory
		for(vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
			delete *it;
		// Then reset the container
		objects.clear();
	}
	
	int save(char * fileName) {
		// Save your scene to a file somehow ...
		return 0;
	}
	void load(char * fileName) {
		// To avoid memory leaks, we run clear, which deletes any old objects first.
		this->clear();
		// load scene from file here ...
	}

};

#endif