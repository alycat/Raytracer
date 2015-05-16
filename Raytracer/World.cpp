#include "World.h"
#include <ppl.h>

using namespace Concurrency;

World::World(void){
	tree = new KDNode();
	pMap = new PhotonMap();
}

World::~World(void){
	for (int i = 0; i < objectList.size(); ++i){
		delete objectList[i];
	}
	objectList.clear();

	for (int l = 0; l < lightList.size(); ++l){
		delete lightList[l];
	}
	lightList.clear();
	if (tree){
		delete tree;
	}
	tree = nullptr;
	if (pMap){
		delete pMap;
	}
	pMap = nullptr;
}

void World::add(Object* obj){
	objectList.push_back(obj);
}

void World::add(LightSource* light){
	lightList.push_back(light);
}

void World::transform(Object obj){

}

//Go through all objects in world and transform them
void World::transformAllObjects(Matrix matrix){
	for (int i = 0; i < objectList.size(); ++i){
		objectList[i]->transform(matrix);
	}

	
}

void World::initTree(){
	BoundingBox box = BoundingBox();
	box.box = { -10, 10, 10, -10, 10, -5 };
//	box.box = { -25, 25, 19, -25, 40, -25};
	tree->box = box;
	for (int i = 0; i < objectList.size(); ++i){
		tree->objects.push_back(objectList[i]);
	}
	tree->expandBoundingBox();
	tree->build(tree, 0);
}

void World::intersection(IntersectData &id, Point point, pVector normal, LightSource* light, pVector camera){
	id.point = point;
	id.normal = normal.normal;
	id.source = {light->position - point};
	id.source = id.source.normal;
	id.outgoing = { point - light->position };
	id.outgoing = id.outgoing.normal;
	id.reflect = reflect(id.source, id.normal);
	id.light = light;
	id.camera = camera;
}

void World::tracePhoton(Ray ray, Color power, int depth){
	if (depth < max_depth){
		//vector<KDNode*> nodes = tree->getNodes(tree, ray);
		KDNode* node = tree->getNearest(tree, ray);
		Object* object = node->objects[0];

		for (int o = 0; o < node->objects.size(); ++o){
			if (object->getBBox().intersect(ray, 0).distance2(ray.start) > node->objects[o]->getBBox().intersect(ray, 0).distance2(ray.start))
			{
				object = node->objects[o];
			}
		}

		Point temp = object->intersect(ray);
		if (temp < maxPoint){
			pVector norm = object->normal(temp);
			IlluminationModel* mat = object->material;
			float d = mat->kd;
			float s = mat->ks;


			float Xi = ((float)rand() / (RAND_MAX)) + 1;
			Photon *p = new Photon();
			p->position = temp;
			p->p_red = power.r;
			p->p_green = power.g;
			p->p_blue = power.b;
			p->flag = 0;
			p->phi = 0;
			p->theta = 0;
			if (Xi < d){
				//diffuse reflection
				float x = 0, y = 0, z = 0;
				x = rand() % 2 + (-1); //random float between -1 and 1
				y = rand() % 2 + (-1); //random float between -1 and 1
				z = rand() % 2 + (-1); //random float between -1 and 1
				pVector d = { x, y, z };
				d = d.normal;
				Ray diffuseray = { temp, d };
				pMap->store(p);
				this->tracePhoton(diffuseray, power, depth + 1);
			}
			else if (Xi < d + s){
				//specular reflection
				pVector specularVector;
				if (object->k_t > 0){
					specularVector = transmit(ray.direction, object->normal(temp));
				}
				else{
					specularVector = reflect(ray.direction, object->normal(temp));
					specularVector.v.z = -specularVector.v.z;

				}
				Ray specularray = { temp, specularVector };
				this->tracePhoton(specularray, power, depth + 1);
			}
			else{
				pMap->store(p);
			}
		}
	}
}

void World::emitPhotons(){
	for (int l = 0; l < lightList.size(); ++l){
		LightSource* light = lightList[l];
		int emitted = 0;
		float x = 1, y = 1, z = 1;
		if (light->numPhotons > 0){
			while (emitted < light->numPhotons){
				do{
					x = rand() % 2 + (-1); //random float between -1 and 1
					y = rand() % 2 + (-1); //random float between -1 and 1
					z = rand() % 2 + (-1); //random float between -1 and 1
				} while (((x*x) + (y*y) + (z*z)) > 1);
				pVector d = { x, y, z };
				d = d.normal;
				Point p = light->position;
				Ray ray = { p, d };
				tracePhoton(ray, light->color/light->numPhotons, 0);
				emitted++;
			}
			//pMap->scalePower(1 / light->numPhotons);
		}
	}
	pMap->balance();
}


Light World::spawn(Ray ray, int depth)
{
	Light light = { background };
	if (tree->box.hit(ray)){
		float closest = sqrt(myMax) / 3;
		Point pClosest = maxPoint;
		vector<KDNode*> nearest;
		nearest = tree->getNodes(tree, ray);
		parallel_for(int(0), (int)nearest.size(), [&](int n)
		{
			parallel_for(int(0), (int)nearest[n]->objects.size(), [&](int i)
			{
					Point temp = nearest[n]->objects[i]->intersect(ray);
					float distance = temp.distance(ray.start);
					cout << "Distance: " << distance << endl;
					if (distance < closest && distance > 0.1){
						IntersectData id;
						light = { black };
						parallel_for(int(0), (int)lightList.size(), [&](int l)
						{
							pVector N = nearest[n]->objects[i]->normal(temp);
							this->intersection(id, temp, N, lightList[l], ray.direction);
							Ray shadow = { lightList[l]->position, { temp - lightList[l]->position } };
							shadow.direction = shadow.direction.normal;

							if (intersection(shadow, i, nearest[n]->objects[i]))
							{
								light = light + nearest[n]->objects[i]->material->illuminate(id);
								pVector I = ray.direction.normal;

								if (depth < max_depth)
								{
									if (nearest[n]->objects[i]->k_r > 0){
										Ray reflection = { temp, reflect(I, N) };
										reflection.direction.v.z = reflection.direction.v.z * -1;
										Light r = spawn(reflection, depth + 1);
										r.irradiance = r.irradiance * nearest[n]->objects[i]->k_r;
										//r.irradiance.forcePositive();
										light = light + r;
									}
									if (nearest[n]->objects[i]->k_t > 0)
									{
										Ray transray = { temp, transmit(I, N) };
										Light t = spawn(transray, depth + 1);
										t.irradiance = t.irradiance * nearest[n]->objects[i]->k_t;
										light = light + t;
									}
								}
							}
						});
						closest = distance;
					}
			});
		});
	}
	return light;
}





Light World::spawnPhoton(Ray ray, int depth)
{
	Light light = { background };
	if (tree->box.hit(ray)){
		float closest = sqrt(myMax) / 3;
		Point pClosest = maxPoint;
		vector<KDNode*> nearest;
		nearest = tree->getNodes(tree, ray);
		parallel_for(int(0), (int)nearest.size(), [&](int n)
		{
			parallel_for(int(0), (int)nearest[n]->objects.size(), [&](int i)
			{
				Point temp = nearest[n]->objects[i]->intersect(ray);
				float distance = temp.distance(ray.start);
				cout << "Distance: " << distance << endl;
				if (distance < closest && distance > 0.0){
					IntersectData id;
					light = { black };
					vector<Photon*> photons = pMap->getPhotons(temp);
					int photonnumber = photons.size();
					cout << photonnumber << endl;
					parallel_for(int(0), (int)photons.size(), [&](int p)
					{
						pVector N = nearest[n]->objects[i]->normal(temp);
						Color pColor = { photons[p]->p_red, photons[p]->p_green, photons[p]->p_blue };
						Light pLight = { pColor };
						LightSource* pSource = new LightSource{ photons[p]->position, pColor, pLight, 0 };
						this->intersection(id, temp, N, pSource, ray.direction);
					//	Ray shadow = { photons[p]->position, { temp - photons[p]->position } };
					//	shadow.direction = shadow.direction.normal;

					//	if (intersection(shadow, i, nearest[n]->objects[i]))
					//	{
						light = light + pLight;
							//light = light + nearest[n]->objects[i]->material->illuminate(id);
							//pVector I = ray.direction.normal;
							/*
							if (depth < max_depth)
							{
								if (nearest[n]->objects[i]->k_r > 0){
									Ray reflection = { temp, reflect(I, N) };
									reflection.direction.v.z = reflection.direction.v.z * -1;
									Light r = spawn(reflection, depth + 1);
									r.irradiance = r.irradiance * nearest[n]->objects[i]->k_r;
									light = light + r;
								}
								if (nearest[n]->objects[i]->k_t > 0)
								{
									Ray transray = { temp, transmit(I, N) };
									Light t = spawn(transray, depth + 1);
									t.irradiance = t.irradiance * nearest[n]->objects[i]->k_t;
									light = light + t;
								}
							}*/
						//}



					});
					closest = distance;
				}
			});
		});
	}
	return light;
}




COLORREF World::trace(Ray ray){
	Color c = background;
	float closest = sqrt(myMax)/3;
	Point pClosest = maxPoint;
	parallel_for (int(0), (int)objectList.size(), [&](int i){
		Point temp = objectList[i]->intersect(ray);
		float distance = temp.distance(ray.start);
		//cout << "Distance: " << distance << endl;
		if (distance < closest){
			c = objectList[i]->color;
			closest = distance;
		}
	});
	return c.getColor;
}

bool World::intersection(Ray ray, int index, Object* obj){
	float closest = sqrt(myMax) / 3;
	Point pClosest = obj->intersect(ray);
	float original = pClosest.distance2(ray.start);
	vector<KDNode*> nearest = tree->getNodes(tree, ray);
	//nearest = tree->traverse(tree);
	parallel_for(int(0), (int)nearest.size(), [&](int n)
	{
		parallel_for(int(0), (int)nearest[n]->objects.size(), [&](int i){
				Point temp = nearest[n]->objects[i]->intersect(ray);
				float dis = temp.distance2(ray.start);
				if (dis < closest && original - dis > 0.1){
					closest = dis;
				}
		});
	});
	if ((closest < sqrt(myMax) / 3)){
		return false;
	}
	return true;
}