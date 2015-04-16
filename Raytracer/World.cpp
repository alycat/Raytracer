#include "World.h"

World::World(void){
	tree = new KDNode();
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
	box.box = { -5, 5, 5, -5, 30, -30 };
	tree->box = box;
	tree->objects = objectList;
	tree->build(objectList, 0);
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

Light World::spawn(Ray ray, int depth){
	Light light = {background};
	//if (tree->hit(tree, ray)){
		float closest = sqrt(myMax) / 3;
		Point pClosest = maxPoint;
		for (int i = 0; i < objectList.size(); ++i){
			Point temp = objectList[i]->intersect(ray);
			float distance = temp.distance(ray.start);
			cout << "Distance: " << distance << endl;
			if (distance < closest){
				IntersectData id;
				light = { black };
				for (int l = 0; l < lightList.size(); ++l){
					this->intersection(id, temp, objectList[i]->normal(temp), lightList[l], ray.direction);
					Ray shadow = { lightList[l]->position, { temp - lightList[l]->position } };
					shadow.direction = shadow.direction.normal;
					if (intersection(shadow, i)){
						light = light + objectList[i]->material->illuminate(id);
						if (depth < max_depth){
						if (objectList[i]->k_r > 0){
							pVector L = ray.direction;
							L = L.normal;
							pVector N = objectList[i]->normal(temp);
							Ray reflection = { temp, reflect(L, N ) };
							reflection.direction.v.z = reflection.direction.v.z * -1;
							Light r = spawn(reflection, depth + 1);
							r.irradiance = r.irradiance * objectList[i]->k_r;
							light = light + r;
							if (light.irradiance.r < 0 || light.irradiance.g < 0 || light.irradiance.b < 0){
							}
						}
						if (objectList[i]->k_t > 0){
							pVector I = { lightList[i]->position - temp };
							pVector N = objectList[i]->normal(temp);
							Point p = { 0, 0, 1 };
							Ray transray = { temp + p, transmit(N, I) };
							Light t = spawn(transray, depth + 1);
							t.irradiance = t.irradiance * objectList[i]->k_t;
							light = light + t;
						}
							
					}
						}
					
					closest = distance;
					
				}
			}
		//}
	}
	return light;
}

COLORREF World::trace(Ray ray){
	Color c = background;
	float closest = sqrt(myMax)/3;
	Point pClosest = maxPoint;
	for (int i = 0; i < objectList.size(); ++i){
		Point temp = objectList[i]->intersect(ray);
		float distance = temp.distance(ray.start);
		cout << "Distance: " << distance << endl;
		if (distance < closest){
			c = objectList[i]->color;
			closest = distance;
		}
	}
	return c.getColor;
}

bool World:: intersection(Ray ray, int index){
	float closest = sqrt(myMax) / 3;
	Point pClosest = objectList[index]->intersect(ray);
	float original = pClosest.distance(ray.start);
	for (int i = 0; i < objectList.size(); ++i){
	if (index != i){
			Point temp = objectList[i]->intersect(ray);
			float distance = temp.distance(ray.start);
			if (distance < original){
				closest = distance;
				original = distance;
			}
		}
	}
	if (closest < sqrt(myMax) / 3){
		return false; 
	}
	return true;
}