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
		if (node->left && node->right){
			if (node->left->objects.size() > 0 || node->right->objects.size() > 0){
				bool hitleft = hit(node->left, ray);
				bool hitright = hit(node->right, ray);
				return (hitleft || hitright);
			}
		}
		else{
			for (int i = 0; i < node->objects.size(); ++i){
				if (node->objects[i]->hit(ray)){
					return true;
				}
			}
		}
		return false;
	}
	return false;
}

KDNode* KDNode::build(vector<Object*>&tris, int depth) const{
	KDNode* node = new KDNode();
	node->objects = tris;
	node->left = nullptr;
	node->right = nullptr;
	if (tris.size() == 0){
		return node;
	}
	if (tris.size() == 1){
		node->box = tris[0]->getBBox();
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->objects = vector<Object*>();
		node->right->objects = vector<Object*>();
		return node;
	}
	node->box = tris[0]->getBBox();
	Point midPoint = origin;
	for (int i = 1; i < tris.size(); ++i){
		node->box.expand(tris[i]->getBBox());
		midPoint = midPoint + tris[i]->getMidPoint();
	}
	midPoint = midPoint / tris.size();
	vector<Object*> left_tris;
	vector<Object*> right_tris;
	int axis = node->box.longestAxis();
	for (int i = 0; i < tris.size(); ++i){
		switch (axis){
		case 0:
			midPoint.x >= tris[i]->getMidPoint().x ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		case 1:
			midPoint.y >= tris[i]->getMidPoint().y ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		case 2:
			midPoint.z >= tris[i]->getMidPoint().z ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		}
	}

	if (left_tris.size() == 0 && right_tris.size() > 0){
		left_tris = right_tris;
	}
	else if (right_tris.size() == 0 && left_tris.size() > 0){
		right_tris = left_tris;
	}
	int matches = 0;
	for (int l = 0; l < left_tris.size(); ++l){
		for (int r = 0; r < right_tris.size(); ++r){
			if (left_tris[l] == right_tris[r]){
				matches++;
			}
		}
	}
	if ((float)matches / left_tris.size() < 0.5 && (float)matches / right_tris.size() < 0.5){
		node->left = build(left_tris, depth + 1);
		node->right = build(right_tris, depth + 1);
	}
	else{
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->objects = vector<Object*>();
		node->right->objects = vector<Object*>();
	}
	return node;
}

void KDNode::build(KDNode* head){
	if (head->objects.size() < 10){ //if terminated
		return;
	}
	
	//find optimal split
	Point midPoint = origin;
	vector<float> x;
	vector<float> y;
	vector<float> z;
	for (int i = 0; i < head->objects.size(); ++i){
		Point p = head->objects[i]->getMidPoint();
		midPoint = midPoint + p;
		x.push_back(p.x);
		y.push_back(p.y);
		z.push_back(p.z);
	}
	int axis = midPoint.x > midPoint.y ? (midPoint.x > midPoint.z ? 0 : 2) : (midPoint.y > midPoint.z ? 1 : 2);
	int median = 0;
	switch (axis){
	case 0:
		median = FindMedian(x);
		break;
	case 1:
		median = FindMedian(y);
		break;
	case 2:
		median = FindMedian(z);
		break;
	}
	head->left = new KDNode();
	head->right = new KDNode();

	for (int i = 0; i < head->objects.size(); ++i){
		Point p = head->objects[i]->getMidPoint();
		switch (axis){
		case 0:
			if (p.x < median){ head->left->objects.push_back(objects[i]); }
			else{ head->right->objects.push_back(objects[i]); }
			break;
		case 1:
			if (p.y < median){ head->left->objects.push_back(objects[i]); }
			else{ head->right->objects.push_back(objects[i]); }
			break;
		case 2:
			if (p.z < median){ head->left->objects.push_back(objects[i]); }
			else{ head->right->objects.push_back(objects[i]); }
			break;
		}
	}
	
	build(head->left);
	build(head->right);
}

void KDNode::sort(){
}

float KDNode::FindMedian(vector<float> f){
	size_t n = f.size();
	nth_element(f.begin(), f.begin() + n, f.end());
	return f[n];
}