#ifndef _OBJECT_H
#define _OBJECT_H

#include "Global.h"
#include <iostream>
#include "Phong.h"
#include "Checker.h"
#include "Rayleigh.h"
#include "BoundingBox.h"

using namespace std;

class Object{
public:
	
	IlluminationModel* material;
	Color color;
	Light light;
	Matrix vM;
	float k_r, k_t;
	Object(void);
	Object(Color c);
	virtual ~Object(void);
	virtual Point intersect(Ray ray);
	virtual void transform(Matrix matrix);
	virtual pVector normal(Point intersection);
	virtual Point getMidPoint();
	virtual BoundingBox getBBox();
	virtual bool hit(Ray ray);
};

#endif