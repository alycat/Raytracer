#include "PhotonMap.h"

PhotonMap::PhotonMap(){
	head = new Node();
	head->left = nullptr;
	head->right = nullptr;
}

PhotonMap::~PhotonMap(){

}

void PhotonMap::balance(Node* node, int depth){
	if (node){
		vector<Photon*> points = node->photonList;
		float size = points.size();
		node->radius = 0;
		if (depth < 20 && size > 500){
			Point abs_Axis = origin;
			Point axis = origin;
			vector<float> x(size);
			vector<float> y(size);
			vector<float> z(size);
			Point smallest = origin;
			Point largest = origin;
			for (int p = 0; p < size; ++p){
				Point _p = node->photonList[p]->position;
				Point tmp = { std::abs(_p.x), std::abs(_p.y), std::abs(_p.z) };
				if (_p.x < smallest.x){
					smallest.x = _p.x;
				}
				if (_p.x > largest.x){
					largest.x = _p.x;
				}
				if (_p.y < smallest.y){
					smallest.y = _p.y;
				}
				if (_p.y > largest.y){
					largest.y = _p.y;
				}
				if (_p.z < smallest.z){
					smallest.z = _p.z;
				}
				if (_p.z > largest.z){
					largest.z = _p.z;
				}
				x[p] = _p.x;
				y[p] = _p.y;
				z[p] = _p.z;
			}
			abs_Axis = largest - smallest;
			abs_Axis.abs();
			float median = 0;
			if (abs_Axis.x > abs_Axis.y){
				if (abs_Axis.x > abs_Axis.z){
					node->axis = 0;
				}
				else{
					node->axis = 2;
				}
			}
			else{
				if (abs_Axis.y > abs_Axis.z){
					node->axis = 1;
				}
				else{
					node->axis = 2;
				}
			}
			//node->axis = (abs_Axis.x > abs_Axis.y) ? ((abs_Axis.x > abs_Axis.z) ? 0 : 2) : ((abs_Axis.y > abs_Axis.z) ? 1 : 2);
			node->right = new Node();
			node->left = new Node();
			node->right->left = nullptr;
			node->right->right = nullptr;
			node->left->left = nullptr;
			node->left->right = nullptr;
			node->right->box = node->box;
			node->left->box = node->box;
			switch (node->axis){
			case 0:
				median = FindMedian(x);
				node->radius = std::abs(abs_Axis.x);
				node->right->box.box.left = median;
				node->left->box.box.right = median;
				break;
			case 1:
				median = FindMedian(y);
				node->radius = std::abs(abs_Axis.y);
				node->right->box.box.bottom = median;
				node->left->box.box.top = median;
				break;
			case 2:
				median = FindMedian(z);
				node->radius = std::abs(abs_Axis.z);
				node->right->box.box.front = median;
				node->left->box.box.back = median;
				break;
			default:
				node->radius = 1;
				break;
			}
			node->median = median;
			
			for (int p = 0; p < size; ++p){
				switch (node->axis){
				case 0:
					if (points[p]->position.x < median){
						node->left->photonList.push_back(points[p]);
					}
					else{
						node->right->photonList.push_back(points[p]);
					}
					break;
				case 1:
					if (points[p]->position.y < median){
						node->left->photonList.push_back(points[p]);
					}
					else{
						node->right->photonList.push_back(points[p]);
					}
					break;
				case 2:
					if (points[p]->position.z < median){
						node->left->photonList.push_back(points[p]);
					}
					else{
						node->right->photonList.push_back(points[p]);
					}
					break;
				}
			}

			this->balance(node->left, depth + 1);
			this->balance(node->right, depth + 1);
		}
		else{
			if (size > 1){

				Point abs_Axis = origin;
				Point axis = origin;
				vector<float> x(size);
				vector<float> y(size);
				vector<float> z(size);
				Point smallest = origin;
				Point largest = origin;
				for (int p = 0; p < size; ++p){
					Point _p = node->photonList[p]->position;
					Point tmp = { std::abs(_p.x), std::abs(_p.y), std::abs(_p.z) };
					if (_p.x < smallest.x){
						smallest.x = _p.x;
					}
					if (_p.x > largest.x){
						largest.x = _p.x;
					}
					if (_p.y < smallest.y){
						smallest.y = _p.y;
					}
					if (_p.y > largest.y){
						largest.y = _p.y;
					}
					if (_p.z < smallest.z){
						smallest.z = _p.z;
					}
					if (_p.z > largest.z){
						largest.z = _p.z;
					}
					x[p] = _p.x;
					y[p] = _p.y;
					z[p] = _p.z;
				}
				abs_Axis = largest - smallest;
				abs_Axis.abs();
				float median = 0;
				if (abs_Axis.x > abs_Axis.y){
					if (abs_Axis.x > abs_Axis.z){
						node->axis = 0;
					}
					else{
						node->axis = 2;
					}
				}
				else{
					if (abs_Axis.y > abs_Axis.z){
						node->axis = 1;
					}
					else{
						node->axis = 2;
					}
				}
				switch (node->axis){
				case 0:
					median = FindMedian(x);
					node->radius = std::abs(abs_Axis.x);
					break;
				case 1:
					median = FindMedian(y);
					node->radius = std::abs(abs_Axis.y);
					break;
				case 2:
					median = FindMedian(z);
					node->radius = std::abs(abs_Axis.z);
					break;
				default:
					node->radius = 1;
					break;
				}
				node->median = median;
			}
		}
	}
}

void PhotonMap::balance(){
	head->left = nullptr;
	head->right = nullptr;
	head->photonList = photons;
	balance(head, 0);
}

void PhotonMap::store(Photon* p){
	photons.push_back(p);
}

void PhotonMap::scalePower(float pe){
	for (int i = 0; i < photons.size(); ++i){
		photons[i]->p_red *= pe;
		photons[i]->p_green *= pe;
		photons[i]->p_blue *= pe;
	}
}

vector<Photon*> PhotonMap::getPhotons(Point p, Ray ray){
	vector<Photon*> photons;
	Node* nearestNode = head->getNode(p, ray, head);
	for (int e = 0; e < nearestNode->photonList.size(); ++e){
		photons.push_back(nearestNode->photonList[e]);
	}
	return photons;
}

Light PhotonMap::irradinceEstimate(Point p, Ray ray){
	if (photons.size() > 0){
		if (head){
			Node* nearestNode = head->getNode(p, ray, head);
			Color c = { 1, 1, 1 };
			float r = 0, g = 0, b = 0;
			if (nearestNode){
				for (int e = 0; e < nearestNode->photonList.size(); ++e){
					Photon* photon = nearestNode->photonList[e];
					r += photon->p_red;
					g += photon->p_green;
					b += photon->p_blue;
				};
			}
			return{ { r, g, b } };
		}
	}
	return{ {0,0,0} };
}



Light PhotonMap::n_nearestPhotons(Point p, Ray ray, int n, float radius){
	std::priority_queue<PhotonCompare, std::vector<PhotonCompare>, std::less<PhotonCompare>> photons;

	Node* node = this->head->getNode(p, ray, this->head);
	
	for (int i = 0; i < node->photonList.size(); ++i){
		Photon* p_curr = node->photonList[i];
		if (p_curr->position.distance2(p) < radius * radius){
			if (photons.size() < n){
				photons.push(PhotonCompare(p_curr, p_curr->position.distance2(p)));
			}
			else{
				PhotonCompare p_cmp = photons.top();
				PhotonCompare p_cmp_curr = PhotonCompare(p_curr, p_curr->position.distance2(p));
				if (p_cmp_curr < p_cmp){
					photons.pop();
					photons.push(p_cmp_curr);
				}
			}
		}
	}
	Color color = black;
	while (!photons.empty()){
		PhotonCompare temp = photons.top();
		Color tmp_col = { temp.photon->p_red, temp.photon->p_green, temp.photon->p_blue };
		color = color + tmp_col;
		photons.pop();
	}
	int size = photons.size();
	return{ color };
}