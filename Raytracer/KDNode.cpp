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
		if (root->box.hit(ray)){
			Point entry = box.intersect(ray, 0);
			Point exit = box.intersect(ray, 1);
			float a, b, s;
			a = b = s = 0;
			s = root->splitAxis.value;
			int axis = root->splitAxis.axis;
			switch (root->splitAxis.axis){
			case 0:
				a = entry.x;
				b = exit.x;
				break;
			case 1:
				a = entry.y;
				b = exit.y;
				break;
			case 2:
				a = entry.z;
				b = exit.z;
			//	s = -s;
				break;
			}

			if ((a <= s)){
				if ((b < s)){	
					if (root->left){
						return getNodes(root->left, ray);
					}
					else
					{
						return{ root };
					}
				}
				else if (b == s){
					float v = rand();
					if (v < 0.5){
						if (root->right){
							getNodes(root->right, ray);
						}
						else{
							return{ root };
						}
					}
					else{
						if (root->left){
							getNodes(root->left, ray);
						}
						else{
							return{ root };
						}
					}
				}
				else{
					if (root->left && root->right){
						vector<KDNode*> n = getNodes(root->left, ray);
						vector<KDNode*> f = getNodes(root->right, ray);
						n.insert(n.end(), f.begin(), f.end());
						return n;
					}
					else{
						return{ root };
					}
				}
			}
			else
			{
				if ((b > s)){
					if (root->right){
						return getNodes(root->right, ray);
					}
					else{
						return{ root };
					}
				}
				else{
					if (root->left && root->right){
						vector<KDNode*> n = getNodes(root->left, ray);
						vector<KDNode*> f = getNodes(root->right, ray);
						n.insert(n.end(), f.begin(), f.end());
						return n;
					}
					else{
						return{ root };
					}
				}
			}
		}
	}
	return{};
}

void KDNode::expandBox(Object* obj){
	BoundingBox objBox = obj->getBBox();
	if (objBox.box.right > box.box.right){
		box.box.right = objBox.box.right;
	}
	if (objBox.box.left < box.box.left){
		box.box.left = objBox.box.left;
	}
	if (objBox.box.top > box.box.top){
		box.box.top = objBox.box.top;
	}
	if (objBox.box.bottom < box.box.bottom){
		box.box.bottom = objBox.box.bottom;
	}
	if (objBox.box.front < box.box.front){
		box.box.front = objBox.box.front;
	}
	if (objBox.box.back > box.box.back){
		box.box.back = objBox.box.back;
	}
}


void KDNode::expandBoundingBox(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->getBBox().box.right > box.box.right){
			box.box.right = objects[i]->getBBox().box.right;
		}
		if (objects[i]->getBBox().box.left < box.box.left){
			box.box.left = objects[i]->getBBox().box.left;
		}
		if (objects[i]->getBBox().box.top > box.box.top){
			box.box.top = objects[i]->getBBox().box.top;
		}
		if (objects[i]->getBBox().box.bottom < box.box.bottom){
			box.box.bottom = objects[i]->getBBox().box.bottom;
		}
		if (objects[i]->getBBox().box.front < box.box.front){
			box.box.front = objects[i]->getBBox().box.front;
		}
		if (objects[i]->getBBox().box.back > box.box.back){
			box.box.back = objects[i]->getBBox().box.back;
		}
	}
}

void KDNode::build(KDNode* head, int d){
	if (head){
		if (head->objects.size() > 8 && d < 30){ //if terminated
			int size = head->objects.size();
			vector<float> x(size);
			vector<float> y(size);
			vector<float> z(size);
			float width = abs(head->box.box.right - head->box.box.left);
			float height = abs(head->box.box.top - head->box.box.bottom);
			float depth = abs(head->box.box.back - head->box.box.front);

			for (int i = 0; i < size; ++i){
				Point p = head->objects[i]->getMidPoint();
				x[i] = p.x;
				y[i] = p.y;
				z[i] = p.z;
			}
			

			if (width > height){
				if (width > depth){
					head->splitAxis.axis = 0;
				}
				else{
					head->splitAxis.axis = 2;
				}
			}
			else{
				if (height > depth){
					head->splitAxis.axis = 1;
				}
				else{
					head->splitAxis.axis = 2;
				}
			}
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
				head->left->box.box.right = head->splitAxis.value + 0.1;
				head->right->box.box.left = head->splitAxis.value - 0.1;
				head->left->box.box.left = head->splitAxis.value - 0.1;
				head->right->box.box.right = head->splitAxis.value + 0.1;
				break;
			case 1:
				head->splitAxis.value = FindMedian(y);
				head->left->box.box.top = head->splitAxis.value;
				head->right->box.box.bottom = head->splitAxis.value;
				head->left->box.box.bottom = head->splitAxis.value;
				head->right->box.box.top = head->splitAxis.value;
				break;
			case 2:
				head->splitAxis.value = FindMedian(z);
				head->left->box.box.back = head->splitAxis.value;
				head->right->box.box.front = head->splitAxis.value;
				head->left->box.box.front = head->splitAxis.value;
				head->right->box.box.back = head->splitAxis.value;
				break;
			}
			
			for (int i = 0; i < size; ++i){
				Point p = head->objects[i]->getMidPoint();
				switch (head->splitAxis.axis){
				case 0:
					if (p.x < head->splitAxis.value)
					{ 
						head->left->objects.push_back(head->objects[i]);
						head->left->expandBox(head->objects[i]);
					}
					else{ 
						head->right->objects.push_back(head->objects[i]);
						head->right->expandBox(head->objects[i]);
					}
					break;
				case 1:
					if (p.y < head->splitAxis.value){
						head->left->objects.push_back(head->objects[i]); 
						head->left->expandBox(head->objects[i]);
					}
					else{ 
						head->right->objects.push_back(head->objects[i]); 
						head->right->expandBox(head->objects[i]);
					}
					break;
				case 2:
					if (p.z >= head->splitAxis.value){
						head->right->objects.push_back(head->objects[i]);
						head->right->expandBox(head->objects[i]);
					}
					else{ 
						head->left->objects.push_back(head->objects[i]);
						head->left->expandBox(head->objects[i]);
					}
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