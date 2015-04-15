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
	KDNode* build(vector<Object*> &t, int d) const;
	bool hit(KDNode* node, const Ray& ray) const;
};
#endif