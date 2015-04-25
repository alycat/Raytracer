#ifndef _KDNODE_H
#define _KDNODE_H

#include "Object.h"

class KDNode{
public:
	BoundingBox box;
	KDNode();
	~KDNode();
	KDNode *left;
	KDNode *right;
	vector<Object*> objects;
	KDNode* build(vector<Object*> &t, float d) const;
	KDNode getNode(vector<Object*> all, BoundingBox B);
	bool Terminal(vector<Object*> L, BoundingBox B);
	bool hit(KDNode* node, const Ray& ray) const;
};
#endif