#ifndef _KDNODE_H
#define _KDNODE_H

#include "Object.h"
#include <algorithm>
#include <functional>
class KDNode{
public:
	BoundingBox box;
	KDNode();
	~KDNode();
	KDNode *left;
	KDNode *right;
	vector<Object*> objects;
	void resizeBox();
	KDNode* build(vector<Object*> &t, int d) const;
	void build(KDNode* head, int d);
	vector<KDNode*> traverse(KDNode* root);
	KDNode* getNode(KDNode* root, Ray ray);
	bool Terminal(vector<Object*> L, BoundingBox B);
	bool hit(KDNode* node, const Ray& ray) const;
	void sort();
	float FindMedian(vector<float>&f);
};
#endif