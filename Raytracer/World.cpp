#include "World.h"
#include <ppl.h>
#include <algorithm>

using namespace Concurrency;
	
World::World(void){
	tree = new KDNode();
	pMap = new PhotonMap();
	c_pMap = new PhotonMap();
	s_pMap = new PhotonMap();
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
//	box.box = { -10, 10, 10, -10, 10, -5 };
	box.box = { -25, 25, 19, -25, 40, -25};
	box.box = { -25, 25, 30, -25, 40, -25 };
	tree->box = box;
	for (int i = 0; i < objectList.size(); ++i){
		tree->objects.push_back(objectList[i]);
	}
	tree->expandBoundingBox();
	tree->build(tree, 0);
	pMap->head->box = box;
	c_pMap->head->box = box;
	s_pMap->head->box = box;
}

void World::intersection(IntersectData &id, Point point, pVector normal, LightSource* light, pVector camera, Point center){
	id.point = point;
	id.normal = normal.normal;
	id.source = {light->position - point};
	id.source = id.source.normal;
	id.outgoing = { point - light->position };
	id.outgoing = id.outgoing.normal;
	id.reflect = reflect(id.source, id.normal);
	id.light = light;
	id.camera = camera;
	id.center = center;
}

void World::tracePhoton(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map, Point initialSource){
	if (depth < 10){
		float maxDis = sqrt(myMax) / 3;
		Object* object = objectList[0];

		Point closest = object->intersect(ray);
		int index = 1;
		
		for (int o = 1; o < objectList.size(); ++o){
			Object* tempObj = objectList[o];
			Point tempPnt = objectList[o]->intersect(ray);
			if (tempPnt.distance2(ray.start) < closest.distance2(ray.start)){
				object = tempObj;
				closest = tempPnt;
			}
		}
		
		if (closest.distance2(initialSource) < 30){
			pVector norm = object->normal(closest);
			Point test = maxPoint;
			cout << test.x << endl;
			IlluminationModel* mat = object->material;
			float d = object->material->kd;
			float r = object->k_r;
			float t = object->k_t;
			float Xi = ((float)rand() / (float)(RAND_MAX));
			float s = r + t; 
			if (t > 0 || r > 0){
		//		d = 0; s = 1;
			}
			Photon *p = new Photon{ closest, power.r, power.g, power.b, 0, 0, 0 };
			pVector N = object->normal(closest);
			pVector I = ray.direction;
			float factor = 100 / (closest.distance2(initialSource));
			factor = 1;
			if (Xi < d){
					float x = 1, y = 1, z = 1;
				//	do{
						x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
						y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
						z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
				//	} while (((x*x) + (y*y) + (z*z)) > 1);

					pVector d = { { x, y, z } };
					Ray ray = { closest, d.normal };
					Color newC = { p->p_red, p->p_green, p->p_blue };
					tracePhoton(ray, power, depth + 1, light, map, initialSource);
					
					p->p_red *= object->color.r;
					p->p_green *= object->color.g;
					p->p_blue *= object->color.b;
					p->p_red *= factor, p->p_green *= factor, p->p_blue *= factor;
					map->store(p);
					
			}
			else if (Xi < s + d){
				
				//specular reflection
				if (object->k_r > 0){
					IntersectData id;
					pVector specularVector;
					specularVector = reflect(I, N);
					specularVector.v.z = -specularVector.v.z;
					this->intersection(id, closest, N, light, ray.direction, object->getMidPoint());
					Ray specularray = { closest, specularVector };
					ray.start = closest;
					this->tracePhoton(specularray, power, depth + 1, light, map, initialSource);
				}
				else{
					pVector specularVector;
					specularVector = transmit(I, N);
					Ray specularray = { closest, specularVector };
					ray.start = closest;
					this->tracePhoton(specularray, power, depth + 1, light, map, initialSource);
				}
			}
			else{ 
					p->p_red *= object->color.r;
					p->p_green *= object->color.g;
					p->p_blue *= object->color.b;
					p->p_red *= factor, p->p_green *= factor, p->p_blue *= factor;
					map->store(p);
			}
			
		}
	}
	//}
}
void World::traceScatter(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map, Point initialSource){
	if (depth < 10){
		float maxDis = sqrt(myMax) / 3;
		Object* object = objectList[0];

		Point closest = object->intersect(ray);
		int index = 1;

		for (int o = 1; o < objectList.size(); ++o){
			Object* tempObj = objectList[o];
			Point tempPnt = objectList[o]->intersect(ray);
			if (tempPnt.distance2(ray.start) < closest.distance2(ray.start)){
				object = tempObj;
				closest = tempPnt;
			}
		}

		if (closest.distance2(initialSource) < 30){
			pVector norm = object->normal(closest);
			Point test = maxPoint;
			cout << test.x << endl;
			IlluminationModel* mat = object->material;
			float d = object->material->kd;
			float r = object->k_r;
			float t = object->k_t;
			float Xi = ((float)rand() / (float)(RAND_MAX));
			float s = r + t;
			if (t > 0 || r > 0){
				//		d = 0; s = 1;
			}
			Photon *p = new Photon{ closest, power.r, power.g, power.b, 0, 0, 0 };
			pVector N = object->normal(closest);
			pVector I = ray.direction;
			float factor = 1 / (closest.distance2(initialSource));
			factor = 1;

			if (Xi < d){
				float x = 1, y = 1, z = 1;
				//	do{
				x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
				y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
				z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
				//	} while (((x*x) + (y*y) + (z*z)) > 1);

				pVector d = { { x, y, z } };
				Ray ray = { closest, d.normal };
				Color newC = { p->p_red, p->p_green, p->p_blue };
				tracePhoton(ray, power, depth + 1, light, map, initialSource);
				pVector lightVector = { light->position - closest };
				float theta = lightVector.normal * N;
				float cosine = theta;
				float cos_color = abs(cosine);
				cos_color *= cos_color;
				p->p_red *= object->color.r * (5 / (abs(2 - ((cos_color + 0.1) / RED))));
				p->p_green *= object->color.g * (5 / (abs(2 - ((cos_color + 0.1) / GREEN))));
				p->p_blue *= object->color.b * (5 / (abs(2 - ((cos_color + 0.1) / BLUE))));
				p->p_red *= factor, p->p_green *= factor, p->p_blue *= factor;
				s_pMap->store(p);

			}
			else if (Xi < s + d){

				//specular reflection
				if (object->k_r > 0){
					IntersectData id;
					pVector specularVector;
					specularVector = reflect(I, N);
					specularVector.v.z = -specularVector.v.z;
					this->intersection(id, closest, N, light, ray.direction, object->getMidPoint());
					Ray specularray = { closest, specularVector };
					ray.start = closest;
					this->traceScatter(specularray, power, depth + 1, light, map, initialSource);
				}
				else{
					pVector specularVector;
					specularVector = transmit(I, N);
				//	specularVector.v.y *= -1;
					Ray specularray = { closest, specularVector };
					ray.start = closest;
					this->traceScatter(specularray, power, depth + 1, light, map, initialSource);
				}
			}
			else{
				pVector lightVector = { light->position - closest };
				float theta = lightVector.normal * N;
				float cosine = theta;
				float cos_color = abs(cosine);
				cos_color *= cos_color;
				p->p_red *= object->color.r * (5 / (abs(2 - ((cos_color + 0.1) / RED))));
				p->p_green *= object->color.g * (5 / (abs(2 - ((cos_color + 0.1) / GREEN))));
				p->p_blue *= object->color.b * (5 / (abs(2 - ((cos_color + 0.1) / BLUE))));
				p->p_red *= factor, p->p_green *= factor, p->p_blue *= factor;
				s_pMap->store(p);
			}

		}
	}
}

/*
void World::tracePhoton(Ray ray, Color power, int depth, LightSource* light, PhotonMap* map){
	if (depth < 20){
		//vector<KDNode*> nodes = tree->getNodes(tree, ray);

		float maxDis = sqrt(myMax) / 3;
		Object* object = objectList[0];
		Point closest = object->intersect(ray);
		for (int o = 1; o < objectList.size(); ++o){
				Point newPoint = objectList[o]->intersect(ray);
				if (closest.distance2(ray.start) > newPoint.distance2(ray.start))
				{
					object = objectList[o];
					closest = newPoint;
				}
		}

		/*
		for (int n = 0; n < nodes.size(); ++n){
		for (int o = 0; o < nodes[n]->objects.size(); ++o){
		if (object->getBBox().intersect(ray, 1))
		}
		}
		
		//Point temp = object->intersect(ray);
		//if (temp < maxPoint){
		pVector norm = object->normal(closest);
		Point test = maxPoint;
		float factor = 1000 / (closest.distance2(light->position));

		if (closest.distance2(ray.start) < 1000){
			
				IlluminationModel* mat = object->material;
				float d = mat->kd;
				float s = mat->ks;
				float r = object->k_r;
				float t = object->k_t;
				if (t > 0 || r > 0)
					d = 0; s = 1;

				float Xi = ((float)rand() / (float)(RAND_MAX));
				Photon *p = new Photon();
				p->position = closest;
				p->p_red = (power.r);
				p->p_green = (power.g);
				p->p_blue = (power.b);
				p->flag = 0;
				p->phi = 0;
				p->theta = 0;
				Color newColor = { p->p_red, p->p_green, p->p_blue };
				pVector N = object->normal(closest);
				pVector I = { closest - light->position };
				I = ray.direction.normal;
				//factor = 1;
				//I = I.normal;
				if (Xi < d && depth > 0){
					//diffuse reflection
					float x = 1, y = 1, z = 1;
					do{
						x = rand() % 2 + (-1); //random float between -1 and 1
						y = rand() % 2 + (-1); //random float between -1 and 1
						//y = -1;
						z = rand() % 2 + (-1); //random float between -1 and 1
					} while ((x*x) + (y*y) + (z*z) > 1);
					pVector d = { x, y, z };
					d = d.normal;
					Ray diffuseray = { closest, d };
					IntersectData id;
					this->intersection(id, closest, N, light, ray.direction);
					/*Light diffuse = object->material->illuminate(id);
					p->p_red *= factor;
					p->p_green *= factor;
					p->p_blue *= factor;
					
					pMap->store(p);
					this->tracePhoton(diffuseray, power, depth + 1, light, map);
				}
				else if (Xi < s){
					 if (r > 0){
					//specular reflection
						pVector specularVector;
						specularVector = reflect(I, N);
						specularVector.v.z = -specularVector.v.z;
						Ray specularray = { closest, specularVector };
						this->tracePhoton(specularray, power, depth + 1, light, map);
					}
					else {
						pVector specularVector;
						specularVector = transmit(I, N);
						Ray specularray = { closest, specularVector };
						//specularVector.v.z = -specularVector.v.z;
						this->tracePhoton(specularray, power, depth + 1, light, map);
					}
				}
				else{
					p->p_red *= factor;
					p->p_green *= factor;
					p->p_blue *= factor;

					map->store(p);
				}
			}
		}
	}
		}
}

*/
void World::emitScatter(){
	for (int l = 0; l < lightList.size(); ++l){
		LightSource* light = lightList[l];
		int emitted = 0;
		float x = 1, y = 1, z = 1;
		if (light->numPhotons > 0){
			while (emitted < light->numPhotons){
				x = 2.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 1
				y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1; //random float between -1 and 1
				z = 1;
				pVector d = { x, y, z };
				Ray ray = { light->position, d.normal };
				traceScatter(ray, light->color, 0, light, s_pMap, light->position);
				//tracePhoton(ray, { 1, 1, 1 }, 0, light);
				emitted++;
			}
			s_pMap->scalePower((1 / (float)light->numPhotons));
		}
	}
	s_pMap->balance();
}

void World::emitCaustic(){
	for (int l = 0; l < lightList.size(); ++l){
		LightSource* light = lightList[l];
		int emitted = 0;
		float x = 1, y = 1, z = 1;
		if (light->numPhotons > 0){
			while (emitted < light->numPhotons * 2){
				//do{
					x = 1 * ((float)rand() / (float)RAND_MAX); //random float between 0 and 1
				//	y = 2 * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 0
					y = -1;
				//	z = 2 * ((float)rand() / (float)RAND_MAX) -1; //random float between 0 and 1
					z = 1;
			//	} while (((x*x) + (y*y) + (z*z)) > 1);
				pVector d = { { x, y, z } };
				Ray ray = { light->position, d.normal };
				tracePhoton(ray, light->color, 0, light, c_pMap, light->position);
		//	do{
					x = 1.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 0
				//	y = 2.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 0
					y = -1;
				//	z = 1.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 0
					z = -1;
		//	} while (((x*x) + (y*y) + (z*z)) > 1);
				d = { { x, y, z } };
				ray = { light->position, d.normal };
				tracePhoton(ray, light->color, 0, light, c_pMap, light->position);
				emitted++;
			}
			c_pMap->scalePower((0.01f / (((float)light->numPhotons)*2)));
		}
	}
	c_pMap->balance();
}


void World::emitPhotons(){
	for (int l = 0; l < lightList.size(); ++l){
		LightSource* light = lightList[l];
		int emitted = 0;
		float x = 1, y = 1, z = 1;
		if (light->numPhotons > 0){
			while (emitted < light->numPhotons){
				do{
					x = 2.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 1
					y = 2.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 1
					z = 2.0f * ((float)rand() / (float)RAND_MAX) -1; //random float between -1 and 1
				} while (((x*x) + (y*y) + (z*z)) > 1);
				pVector d = { x, y, z };
				Ray ray = { light->position, d.normal };
				tracePhoton(ray, light->color, 0, light, pMap, light->position);
				//tracePhoton(ray, { 1, 1, 1 }, 0, light);
				emitted++;
			}
			pMap->scalePower((0.02 / (float)light->numPhotons));
		}
	}
	pMap->balance();
}

void World::setBackgroundColor(Color c){
	backColor.backgroundColor = c;
}

Light World::spawn(Ray ray, int depth)
{
	Light light = { backColor.backgroundColor };
//	Light light = { black };
	Light caustics = { black };
	Light photonLight = { black };
	Light rayleigh = { black };
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
					
					PhotonMap::Node *node = c_pMap->head->getNode(temp, ray, c_pMap->head);
					float radius = node->radius;
					int n_photons = node->photonList.size();
			
					if (distance < closest && distance > 0.1){
						IntersectData id;
						id.center = nearest[n]->objects[i]->getMidPoint();
						Light cLight = c_pMap->n_nearestPhotons(temp, ray, 100, 2.5);
						Light pLight = pMap->n_nearestPhotons(temp, ray, 100, 4);
						Light rLight = s_pMap->n_nearestPhotons(temp, ray, 100, 4);
						caustics = caustics + cLight;
						photonLight = photonLight + pLight;
						
						parallel_for(int(0), (int)lightList.size(), [&](int l)
						{
							//rayleigh = { black };
							light = { black };
							pVector N = nearest[n]->objects[i]->normal(temp);
							this->intersection(id, temp, N, lightList[l], ray.direction, nearest[n]->objects[i]->getMidPoint());
							Ray shadow = { lightList[l]->position, { temp - lightList[l]->position } };
							shadow.direction = shadow.direction.normal;
							rayleigh = rayleigh + rLight;
							if (intersection(shadow, i, nearest[n]->objects[i]))
							{
								Light rLight = nearest[n]->objects[i]->material->illuminate(id);
								
								light = light + rLight;
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
									//	light = r;
									}
									if (nearest[n]->objects[i]->k_t > 0)
									{
										Ray transray = { temp, transmit(I, N) };
										Light t = spawn(transray, depth + 1);
										t.irradiance = t.irradiance * nearest[n]->objects[i]->k_t;
										light = light + t;
										//light = t;
									}
								}

							}
						});
						closest = distance;
					}
				});
			});
	}
	return light + photonLight + caustics + rayleigh;
}





Light World::spawnPhoton(Ray ray, int depth)
{
	Light light = { black };
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
				vector<Photon*> photonList = pMap->getPhotons(temp, ray);

				IntersectData id;
				light = light + c_pMap->irradinceEstimate(temp, ray);
				float r = 0, g = 0, b = 0;
				/*parallel_for(int(0), (int)photonList.size(), [&](int p)
				{
					pVector N = nearest[n]->objects[i]->normal(temp);
					Light pLight = pMap->irradinceEstimate(temp, ray);
					Color pColor = pLight.irradiance;
					LightSource* source = new LightSource{ temp, pColor, pLight, 1 };
					pLight = { { photonList[p]->p_red, photonList[p]->p_green, photonList[p]->p_blue } };
					
					this->intersection(id, temp, N, source, ray.direction);
					Ray shadow = { source->position, { temp - source->position } };
					shadow.direction = shadow.direction.normal;
					//pLight = pMap->irradinceEstimate(temp, ray);
					//light = { black };
						light = light + pLight;
						//light = light + nearest[n]->objects[i]->material->illuminate(id);
						pVector I = ray.direction.normal;
				});*/
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
			if (obj != nearest[n]->objects[i]){
				Point temp = nearest[n]->objects[i]->intersect(ray);
				float dis = temp.distance2(ray.start);
				if (dis < closest && original - dis > 0.1 && nearest[n]->objects[i]->k_t == 0){
					closest = dis;
				}
			}
		});
	});
	if ((closest < sqrt(myMax) / 3)){
		return false;
	}
	return true;
}