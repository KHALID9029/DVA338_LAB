#include "RayTracer.h"
#include "Vec3.h" 
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
RayTracer* initTracer(int width, int height)
{
	RayTracer* rayTracer = new RayTracer(width, height);
	
	//Red
	rayTracer->addSphere(Vec3f(0.0f, 0.0f, -10.0f), 1.0f,
		new Material(
			Vec3f(0.1f, 0.0f, 0.0f),
			Vec3f(0.6f, 0.0f, 0.0f),
			Vec3f(0.6f, 0.6f, 0.6f),
			8.0f
	));

	//Green
	rayTracer->addSphere(Vec3f(2.0f, 0.0f, -11.0f), 0.8f,
		new Material(
			Vec3f(0.0f, 0.1f, 0.0f),   
			Vec3f(0.0f, 0.6f, 0.0f),  
			Vec3f(0.6f, 0.6f, 0.6f),  
			64.0f
	));

	//light
	rayTracer->addLight(Vec3f(-10.0f, 10.0f, 10.0f), 
		Vec3f(1.0f, 1.0f, 1.0f), 
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f));

	//plane
	rayTracer->addPlane(
		Vec3f(0.0f, -1.0f, 0.0f),      
		Vec3f(0.0f, 1.0f, 0.0f),     
		new Material(
			Vec3f(0.02f, 0.02f, 0.1f),
			Vec3f(0.05f, 0.5f, 0.25f),
			Vec3f(0.2f, 0.2f, 0.2f),
			16.0f
		)
	);

	
	return rayTracer;
}

void runTracer(RayTracer* tracer)
{
	tracer->fireRays();
	tracer->toPPM("DVA338_Raytracer_output.ppm");
	tracer->toBMP("DVA338_Raytracer_output.bmp");
}

void destroyTracer(RayTracer* tracer)
{
	delete tracer;
}

int main(int argc, char **argv) {
	RayTracer* tracer = initTracer(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	runTracer(tracer);
	destroyTracer(tracer);
	return 0;
}
