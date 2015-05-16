#ifndef _KDNODE_H
#define _KDNODE_H

#include "Object.h"
#include <algorithm>
#include <functional>
class KDNode{
public:
	struct Axis{
		int axis;
		float value;
	};
	Axis splitAxis;
	BoundingBox box;
	KDNode();
	~KDNode();
	KDNode *left;
	KDNode *right;
	vector<Object*> objects;
	void expandBoundingBox();
	void expandBox(Object* obj);
	void build(KDNode* head, int d);
	vector<KDNode*> traverse(KDNode* root);
	vector<KDNode*> getNodes(KDNode* root, Ray ray);
	KDNode* getNearest(KDNode* root, Ray ray);
	bool Terminal(vector<Object*> L, BoundingBox B);
	bool hit(KDNode* node, const Ray& ray) const;
	void sort();
};
#endif