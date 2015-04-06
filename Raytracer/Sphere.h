#ifndef _SPHERE_H
#define _SPHERE_H

#include "Object.h"

class Sphere :public Object{
public:
	pVector normal(Point intersection) override;
	Point center;
	float radius;
	Sphere(void);
	~Sphere(void);
	Sphere(Color c);
	Sphere(Point cen, float r);
	Sphere(Point cen, float r, Color c);
	Sphere(Point cen, float r, Light l);
	Point intersect(Ray ray);
	void transform(Matrix matrix);
};
#endif