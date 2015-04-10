#ifndef _OBJECT_H
#define _OBJECT_H

#include "Global.h"
#include <iostream>
#include "Phong.h"
#include "Checker.h"

using namespace std;

class Object{
public:
	
	IlluminationModel* material;
	Color color;
	Light light;
	Matrix vM;
	Object(void);
	Object(Color c);
	~Object(void);
	virtual Point intersect(Ray ray);
	virtual void transform(Matrix matrix);
	virtual pVector normal(Point intersection);
};

#endif