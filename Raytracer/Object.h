#ifndef _OBJECT_H
#define _OBJECT_H

#include "Global.h"
#include <iostream>

using namespace std;

class Object{
public:
	Color color;
	Matrix vM;
	Object(void);
	Object(Color c);
	~Object(void);
	virtual Point intersect(Ray ray);
	virtual void transform(Matrix matrix);
};

#endif