#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"

class World{
protected:
	void intersection(IntersectData &id, Point point, pVector normal, LightSource* light);
public:
	vector<Object*> objectList;
	vector<LightSource*> lightList;
	World(void);
	~World(void);
	COLORREF trace(Ray ray);
	void add(Object* obj);
	void add(LightSource* light);
	void transform(Object obj);
	void transformAllObjects(Matrix matrix);
	bool intersection(Ray ray, int index);
	Light spawn(Ray ray);
};
#endif