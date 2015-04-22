/**
	http://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
**/

#include "KDNode.h"
KDNode::KDNode(){

}

KDNode::~KDNode(){
	if (left){
		delete left;
	}
	left = nullptr;
	if (right){
		delete right;
	}
	right = nullptr;
	/*
	for (int i = 0; i < objects.size(); ++i){
		delete objects[i];
	}*/
	objects.clear();
}

bool KDNode::hit(KDNode* node, const Ray& ray) const{
	if (node->box.hit(ray)){
		if (node->left){
			if (node->left->objects.size() > 0){
				return hit(node->left, ray);
			}
		}
		if (node->right){
			if (node->right->objects.size() > 0){
				return hit(node->right, ray);
			}
		}
		else{
			for (int i = 0; i < node->objects.size(); ++i){
				if (node->objects[i]->getBBox().hit(ray)){
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

KDNode* KDNode::build(vector<Object*>&t, BoundingBox b) const{
	KDNode* node = new KDNode();
	node->objects = t;
	node->left = nullptr;
	node->right = nullptr;
	node->box = BoundingBox();

	if (t.size() == 0){
		return node;
	}

	if (t.size() == 1){
		node->box = t[0]->getBBox();
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->objects = vector<Object*>();
		node->right->objects = vector<Object*>();
		return node;
	}

	node->box = t[0]->getBBox();
	Point mid = origin;

	for (int i = 0; i < t.size(); ++i){
		mid = mid + (t[i]->getMidPoint() * (1.0/t.size()));
	}

	vector<Object*> l_t;
	vector<Object*> r_t;

	int axis = node->box.longestAxis();

	for (int i = 0; i < t.size(); ++i){
		switch (axis){
		case 0:
			mid.x >= t[i]->getMidPoint().x ? r_t.push_back(t[i]) : l_t.push_back(t[i]);
			break;
		case 1:
			mid.y >= t[i]->getMidPoint().y ? r_t.push_back(t[i]) : l_t.push_back(t[i]);
			break;
		case 2:
			mid.z >= t[i]->getMidPoint().z ? r_t.push_back(t[i]) : l_t.push_back(t[i]);
			break;
		}
	}

	if (l_t.size() == 0 && r_t.size() > 0){l_t = r_t; }
	else if (r_t.size() == 0 && l_t.size() > 0){ r_t = l_t; }

	int matches = 0;

	for (int i = 0; i < l_t.size(); ++i){
		for (int j = 0; j < r_t.size(); ++j){
			if (l_t[i] == r_t[j]){
				matches++;
			}
		}
	}


	if ((float)matches / l_t.size() < 0.5 && (float)matches / r_t.size() < 0.5){
	//	node->left = build(l_t, d + 1);
		//node->right = build(r_t, d + 1);
	}
	else{
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->objects = vector<Object*>();
		node->right->objects = vector<Object*>();
	}

	return node;
}