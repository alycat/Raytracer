#ifndef _SPHERE_H
#define _SPHERE_H

#include "Object.h"

class Sphere :public Object{
public:
	Point center;
	float radius;
	Sphere(void);
	~Sphere(void);
	Sphere(Color c);
	Sphere(Point cen, float r);
	Sphere(Point cen, float r, Color c);
	Point intersect(Ray ray);
	void transform(Matrix matrix);
};
#endif