#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "Object.h"

class Triangle : public Object{
public:
	Point p1, p2, p3;

	Triangle(void);
	~Triangle(void);
	Triangle(Point a, Point b, Point c);
	Point intersect(Ray ray);
	void transform(Matrix matrix);
};

#endif