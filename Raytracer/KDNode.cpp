/**
	http://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
**/

#include "KDNode.h"
KDNode::KDNode(){
	right = nullptr;
	left = nullptr;
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
	}
	objects.clear();*/
}
vector<KDNode*> KDNode::traverse(KDNode* root){
	if (root){
		if (root->right || root->left){
			vector<KDNode*> l = traverse(root->left);
			vector<KDNode*> r = traverse(root->right);
			l.insert(l.end(), r.begin(), r.end());
			return l;
		}
		else{
			return{ root };
		}
	}
	return{};
}

vector<KDNode*> KDNode::getNodes(KDNode* root, Ray ray){
	if (root){
		/*vector<KDNode*> leaves = traverse(root);
		KDNode* Near = leaves[0];
		for (int i = 1; i < leaves.size(); ++i){
		if (ray.start.distance(leaves[i]->box.intersect(ray, 0)) < ray.start.distance(Near->box.intersect(ray, 0))){
		Near = leaves[i];
		}
		}*/
		Point entry = box.intersect(ray, 0);
		Point exit = box.intersect(ray, 1);
		Point split = origin;
		pVector N = { origin };
		float V = root->splitAxis.axis;
		float F = -V;
		float omega = 0;
		switch (root->splitAxis.axis){
		case 0:
			N.v = { 0, 1, 0 };
			omega = -(V *ray.start.y + F) / (V*ray.direction.v.y);
			break;
		case 1:
			N.v = { 0, 0, 1 };
			omega = -(V *ray.start.z + F) / (V*ray.direction.v.z);
			break;
		case 2:
			N.v = { 1, 0, 0 };
			omega = -(V *ray.start.x + F) / (V*ray.direction.v.x);
			break;
		}
		split = ray.start + (ray.direction.v * omega);
		float a, b, s;
		a = b = s = 0;
		switch (root->splitAxis.axis){
		case 0:
			a = entry.x;
			b = exit.x;
			s = split.x;
			break;
		case 1:
			a = entry.y;
			b = exit.y;
			s = split.y;
			break;
		case 2:
			a = entry.z;
			b = exit.z;
			s = split.x;
			break;
		}
		KDNode* Near = nullptr;
		KDNode* Far = nullptr;
		if ((a < root->splitAxis.value && root->splitAxis.axis != 2)){
			if (root->left){
				Near = root->left;
			}
			else if (root->right){
				Near = root->right;
			}
			if (root->right){
				Far = root->right;
			}
			else if (root->left){
				Far = root->left;
			}
		}
		else{
			if (root->right){
				Near = root->right;
			}
			else if (root->left){
				Near = root->left;
			}
			if (root->left){
				Far = root->left;
			}
			else if (root->right){
				Far = root->right;
			}
		}
		if (Near && Far){
			if (((s < 0 || s > b) && root->splitAxis.axis != 2)){
				if (Near->left || Near->right){
					return getNodes(Near, ray);
				}
				else{
					return{ Near };
				}
			}
			else if ((s < a && root->splitAxis.axis != 2)){
				if (Far->left || Far->left){
					return getNodes(Far, ray);
				}
				else{
					return{ Far };
				}
			}
			else{
				if ((Near->left || Near->right) && (Far->left || Far->right)){
					vector<KDNode*> n = getNodes(Near, ray);
					vector<KDNode*> f = getNodes(Far, ray);
					n.insert(n.end(), f.begin(), f.end());
					return n;
				}
				else if (Near->left || Near->right){
					vector<KDNode*> n = getNodes(Near, ray);
					vector<KDNode*> f = { Far };
					n.insert(n.end(), f.begin(), f.end());
					return n;
				}
				else if (Far->left || Far->right){
					vector<KDNode*> n = { Near };
					vector<KDNode*> f = getNodes(Far, ray);
					n.insert(n.end(), f.begin(), f.end());
					return n;
				}
				else{
					return{ Near, Far };
				}
			}
		}
		else
		{
			return traverse(root);
		}
	}
	return{};
}

void KDNode::resizeBox(){
	/*box.box = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < objects.size(); ++i){
		if (box.box.left > objects[i]->getBBox().box.left){
			box.box.left = objects[i]->getBBox().box.left;
		}
		if (box.box.right < objects[i]->getBBox().box.right){
			box.box.right = objects[i]->getBBox().box.right;
		}
		if (box.box.top < objects[i]->getBBox().box.top){
			box.box.top = objects[i]->getBBox().box.top;
		}
		if (box.box.bottom > objects[i]->getBBox().box.bottom){
			box.box.bottom = objects[i]->getBBox().box.bottom;
		}
		if (box.box.back < objects[i]->getBBox().box.back){
			box.box.back = objects[i]->getBBox().box.back;
		}
		if (box.box.front > objects[i]->getBBox().box.front){
			box.box.front = objects[i]->getBBox().box.front;
		}
	}*/
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

void KDNode::build(KDNode* head, int d){
	if (head){
		if (head->objects.size() > 3 && d < 10){ //if terminated
			int size = head->objects.size();
			//find optimal split
			Point midPoint = origin;
			vector<float> x(size);
			vector<float> y(size);
			vector<float> z(size);
			for (int i = 0; i < size; ++i){
				Point p = head->objects[i]->getMidPoint();
				midPoint = midPoint + p;
				x[i] = p.x;
				y[i] = p.y;
				z[i] = p.z;
			}
			head->splitAxis.axis = midPoint.x > midPoint.y ? (midPoint.x > midPoint.z ? 0 : 2) : (midPoint.y > midPoint.z ? 1 : 2);
			int median = 0;

			head->left = new KDNode();
			head->right = new KDNode();
			head->left->left = nullptr;
			head->right->left = nullptr;
			head->left->right = nullptr;
			head->right->right = nullptr;
			head->left->box.box = head->box.box;
			head->right->box.box = head->box.box;
			switch (head->splitAxis.axis){
			case 0:
				head->splitAxis.value = FindMedian(x);
				head->left->box.box.right = head->splitAxis.value;
				head->right->box.box.left = head->splitAxis.value;
				break;
			case 1:
				head->splitAxis.value = FindMedian(y);
				head->left->box.box.top = head->splitAxis.value;
				head->right->box.box.bottom = head->splitAxis.value;
				break;
			case 2:
				head->splitAxis.value = FindMedian(z);
				head->left->box.box.back = head->splitAxis.value;
				head->right->box.box.front = head->splitAxis.value;
				break;
			}
			
			//head->left->objects = vector<Object*>();
			//head->right->objects = vector<Object*>();
			for (int i = 0; i < size; ++i){
				Point p = head->objects[i]->getMidPoint();
				switch (head->splitAxis.axis){
				case 0:
					if (p.x < head->splitAxis.value)
					{ head->left->objects.push_back(head->objects[i]); }
					else{ head->right->objects.push_back(head->objects[i]); }
					break;
				case 1:
					if (p.y < head->splitAxis.value){ head->left->objects.push_back(head->objects[i]); }
					else{ head->right->objects.push_back(head->objects[i]); }
					break;
				case 2:
					if (p.z > head->splitAxis.value){ head->left->objects.push_back(head->objects[i]); }
					else{ head->right->objects.push_back(head->objects[i]); }
					break;
				}
			}

			build(head->left, d + 1);
			build(head->right, d + 1);
		}
	}
}

void KDNode::sort(){
}

float KDNode::FindMedian(vector<float>& f){
	size_t n = f.size() > 1? f.size() / 2 : 0;
	nth_element(f.begin(), f.begin() + n, f.end());
	return f[n];
}