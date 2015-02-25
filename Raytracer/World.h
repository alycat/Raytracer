#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"
#include <vector>

using namespace std;

class World{
public:
	vector<Object*> objectList;

	World();
	~World();
	COLORREF trace(Ray ray);
	void add(Object* obj);
	void transform(Object obj);
	void transformAllObjects(Matrix matrix);
	void spawn(Ray ray);
};
#endif