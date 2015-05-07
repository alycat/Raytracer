#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"
#include "KDNode.h"

class World{
protected:
	void intersection(IntersectData &id, Point point, pVector normal, LightSource* light, pVector camera);
public:
	vector<Object*> objectList;
	vector<LightSource*> lightList;
	KDNode* tree;
	World(void);
	~World(void);
	COLORREF trace(Ray ray);
	void initTree();
	void add(Object* obj);
	void add(LightSource* light);
	void transform(Object obj);
	void transformAllObjects(Matrix matrix);
	bool intersection(Ray ray, int index, Object* obj);
	Light spawn(Ray ray, int depth);
};
#endif