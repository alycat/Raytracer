#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"

class World{
protected:
	void intersection(IntersectData &id, Point point, pVector normal, pVector incoming, pVector outgoing, vector<LightSource*> lights);
	pVector reflect(pVector L, pVector N);
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
	Color spawn(Ray ray);
};
#endif