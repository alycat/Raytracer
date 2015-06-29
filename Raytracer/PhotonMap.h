#ifndef _PHOTONMAP_H
#define _PHOTONMAP_H

#include "Global.h"
#include "BoundingBox.h"
#include <queue>
#include <functional>

class PhotonMap{
public:
	struct PhotonCompare{
		float distance;
		Photon* photon;
		PhotonCompare(Photon* pho, float dist) :photon(pho), distance(dist){}

		bool operator<(PhotonCompare p) const{
			return distance < p.getDistance();
		}


		float getDistance(){
			return distance;
		}
	};
	struct Node{
		Node* left;
		Node* right;
		BoundingBox box;
		float median;
		int axis;
		float radius;
		vector<Photon*> photonList;

		vector<Node*> traverse(Node* node){
			if (node){
				if (node->left || node->right){
					vector<Node*> l = traverse(node->left);
					vector<Node*> r = traverse(node->right);
					l.insert(l.end(), r.begin(), r.end());
					return l;
				}
				else{
					return{ node };
				}
			}
			return{};
		}

		Node* getNode(Point p, Ray ray, Node* node){
			if (node){
				vector<Node*> leaves = traverse(node);
				if (leaves.size() > 0){
					Node* closest = leaves[0];
					for (int l = 1; l < leaves.size(); ++l){
						if (closest->box.intersect(ray, 0).distance2(ray.start) > leaves[l]->box.intersect(ray, 0).distance2(ray.start)){
							closest = leaves[l];
						}
					}
					return closest;
				}
				else{
					return node;
				}
			}
			return nullptr;
		}

		float getRadius(){
			float x_axis = std::abs(box.box.right) + std::abs(box.box.left);
			float y_axis = std::abs(box.box.top) + std::abs(box.box.bottom);
			float z_axis = std::abs(box.box.back) + std::abs(box.box.front);
			if (x_axis > y_axis){
				if (x_axis > z_axis){
					return x_axis;
				}
				return z_axis;
			}
			if (y_axis > z_axis){
				return y_axis;
			}
			return z_axis;
		}

	};
	void balance(Node* node, int depth);
	vector<Node*> getNodes(Point p, Node* node);
public:
	vector<Photon*> photons;
	Node* head;
	PhotonMap();
	~PhotonMap();
	void store(Photon* p);
	void scalePower(float pe);
	void balance();
	vector<Photon*> getPhotons(Point p, Ray ray);
	Light irradinceEstimate(Point p, Ray ray);
	Light n_nearestPhotons(Point p, Ray ray, int n, float radius);

};

#endif