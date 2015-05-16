#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"
#include "KDNode.h"
#include "PhotonMap.h"

class World{
protected:
	void intersection(IntersectData &id, Point point, pVector normal, LightSource* light, pVector camera);
	void tracePhoton(Ray ray, Color power, int depth);
public:
	vector<Object*> objectList;
	vector<LightSource*> lightList;
	PhotonMap* pMap;
	KDNode* tree;
	World(void);
	~World(void);
	COLORREF trace(Ray ray);
	void initTree();
	void add(Object* obj);
	void add(LightSource* light);
	void emitPhotons();
	void transform(Object obj);
	void transformAllObjects(Matrix matrix);
	bool intersection(Ray ray, int index, Object* obj);
	Light spawn(Ray ray, int depth);
	Light spawnPhoton(Ray ray, int depth);
};
#endif