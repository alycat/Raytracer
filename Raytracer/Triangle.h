#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "Object.h"

class Triangle : public Object{
public:
	pVector vn;
	Point p1, p2, p3;
	pVector normal(Point intersection) override;
	Triangle(void);
	~Triangle(void);
	Triangle(Point a, Point b, Point c, UV uv);
	Triangle(Point a, Point b, Point c, Light l, UV uv);
	Point intersect(Ray ray);
	void transform(Matrix matrix);
};

#endif