#ifndef _KDNODE_H
#define _KDNODE_H

#include "Triangle.h"

class KDNode{
public:
	BoundingBox box;
	KDNode();
	~KDNode();
	KDNode *left;
	KDNode *right;
	vector<Triangle*> triangles;
	KDNode* build(vector<Triangle*> &t, int d) const;
	bool hit(KDNode* node, const Ray& ray, float& t, float& tmin) const;
};
#endif