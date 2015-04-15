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

	for (int i = 0; i < triangles.size(); ++i){
		delete triangles[i];
	}
	triangles.clear();
}

bool KDNode::hit(KDNode* node, const Ray& ray, float& t, float& tmin) const{
	if (node->box.hit(ray)){
		pVector normal;
		bool hit_t = false;
		Point hit_p = origin;
		if (node->left->triangles.size() > 0 || node->right->triangles.size() > 0){
			bool hit_l = hit(node->left, ray, t, tmin);
			bool hit_r = hit(node->right, ray, t, tmin);
			return hit_l || hit_r;
		}
		else{
			for (int i = 0; i < node->triangles.size(); ++i){
				if (node->triangles[i]->intersect(ray) < maxPoint){
					hit_t = true;
					tmin = t;
					hit_p = node->triangles[i]->intersect(ray);
					normal = node->triangles[i]->normal(hit_p);
				}
			}
			if (hit_t){
				return true;
			}
			return false;
		}
	}
	return false;
}

KDNode* KDNode::build(vector<Triangle*>&t, int d) const{
	KDNode* node = new KDNode();
	node->triangles = t;
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
		node->left->triangles = vector<Triangle*>();
		node->right->triangles = vector<Triangle*>();
		return node;
	}

	node->box = t[0]->getBBox();
	Point mid = origin;

	for (int i = 0; i < t.size(); ++i){
		mid = mid + (t[i]->getMidPoint() * (1.0/t.size()));
	}

	vector<Triangle*> l_t;
	vector<Triangle*> r_t;

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
		node->left = build(l_t, d + 1);
		node->right = build(r_t, d + 1);
	}
	else{
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = vector<Triangle*>();
		node->right->triangles = vector<Triangle*>();
	}

	return node;
}