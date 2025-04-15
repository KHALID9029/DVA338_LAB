#include "RayTracer.h"
#include "raytrace_features.h"

void RayTracer::searchClosestHit(const Ray & ray, HitRec & hitRec) {
    hitRec.anyHit = false;

	hitRec.tHit = ray.tClip;

	bool hit = false;
    for (int i = 0; i < this->scene->objects.size(); i++) {
        hit = false;
        hit=this->scene->objects[i]->hit(ray, hitRec);

        if (hit) {
			hitRec.primIndex = i;
        }
    }
}

Vec3f RayTracer::getEyeRayDirection(int x, int y) {
    //Uses a fix camera looking along the negative z-axis
    static float z = -5.0f;		
    static float sizeX = 4.0f; 
    static float sizeY = 3.0f; 
    static float left = -sizeX * 0.5f;
    static float top = sizeY * 0.5f;
    static float dx =  sizeX / float(this->image->getWidth());  
    static float dy =  sizeY / float(this->image->getHeight());

    return Vec3f(left + x * dx, top - y * dy, z).normalize();
}

void RayTracer::fireRays() {
    Ray ray;
    
    for(int x = 0; x < this->image->getWidth(); x++) {
        for(int y = 0; y < this->image->getHeight(); y++) {
            ray.d = RayTracer::getEyeRayDirection(x, y);
            RayTracer::searchClosestHit(ray, ray.hitRec);

            if (ray.hitRec.anyHit) {  

				Vec3f color = Vec3f(0.0f, 0.0f, 0.0f);
				
                for (int i = 0; i < this->scene->lights.size(); i++) {
					color += CalculateColor(ray, ray.hitRec, this->scene->lights[i]);
                }

                this->image->setPixel(x, y, color);
            }
            else {
                this->image->setPixel(x, y, this->bgColor); 
            }
        }
    }
}


Vec3f RayTracer::CalculateColor(const Ray & ray, const HitRec & hitRec, const Light* light) {
	
	Vec3f N = hitRec.n;
	N = N.normalize();
	Vec3f L = (light->position - hitRec.p).normalize();
	Vec3f V = (ray.o - hitRec.p).normalize();
	Vec3f R = (N * (2.0f * N.dot(L)) - L).normalize();

	Vec3f ambient = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f diffuse = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f specular = Vec3f(0.0f, 0.0f, 0.0f);

#if defined(AMBIENT_LIGHTING)
	//Ambient light
	ambient = hitRec.material->Ambient.multCoordwise(light->Ambient);
#endif


	//For shadow rays
	bool inShadow = false;

#if defined(SHADOWS_BLACK) || defined(SHADOWS_AMBIENT)

	Vec3f shadowOrigin = hitRec.p + N * Ray::rayEps; // Added N* Ray::rayEps to avoid self-shadowing and precision errors
	Ray shadowRay(shadowOrigin, L);
    shadowRay.tClip = (light->position - shadowOrigin).len();
	
	HitRec shadowHitRec;
	searchClosestHit(shadowRay, shadowHitRec);
    if (shadowHitRec.anyHit && shadowHitRec.tHit < shadowRay.tClip) {
        inShadow = true;
    }
#endif

	bool skipDiffuseAndSpecular = false;

#if defined(SHADOWS_BLACK)
	if (inShadow) {
		skipDiffuseAndSpecular = true;
		ambient = Vec3f(0.0f, 0.0f, 0.0f);
	}
#elif defined(SHADOWS_AMBIENT)
	if (inShadow) {
		skipDiffuseAndSpecular = true;
	}
#endif

	if (!skipDiffuseAndSpecular) {

#if defined(DIFFUSE_LIGHTING)
		diffuse = hitRec.material->Diffuse.multCoordwise(light->Diffuse) * fmaxf(0.0f, N.dot(L));
#endif

#if defined(SPECULAR_LIGHTING)
		specular = hitRec.material->Specular.multCoordwise(light->Specular) * powf(fmaxf(0.0f, R.dot(V)), hitRec.material->Shininess);
#endif
	}


	Vec3f color = ambient*0.5+ambient;
	color = color + diffuse + specular;

	//Clamp color to [0,1]
	color.x = fminf(fmaxf(color.x, 0.0f), 1.0f);
	color.y = fminf(fmaxf(color.y, 0.0f), 1.0f);
	color.z = fminf(fmaxf(color.z, 0.0f), 1.0f);
	return color;
}