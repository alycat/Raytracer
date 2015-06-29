#ifndef _WORLD_H
#define _WORLD_H


#include "Sphere.h"
#include "Triangle.h"
#include "KDNode.h"
#include "PhotonMap.h"

class World{
protected:
	void intersection(IntersectData &id, Point point, pVector normal, LightSource* light, pVector camera, Point center);
	//void tracePhoton(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map);
//	void traceCaustic(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map, Point initialSource);
	void tracePhoton(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map, Point initialSource);
	void traceScatter(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map, Point initialSource);
public:
	vector<Object*> objectList;
	vector<LightSource*> lightList;
	PhotonMap* pMap, *c_pMap, *s_pMap;
	KDNode* tree;
	World(void);
	~World(void);
	COLORREF trace(Ray ray);
	void initTree();
	void add(Object* obj);
	void add(LightSource* light);
	void emitCaustic();
	void emitScatter();
	void emitPhotons();
	void transform(Object obj);
	void transformAllObjects(Matrix matrix);
	bool intersection(Ray ray, int index, Object* obj);
	void setBackgroundColor(Color c);
	Light spawn(Ray ray, int depth);
	Light spawnPhoton(Ray ray, int depth);
};
#endif