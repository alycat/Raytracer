#include "World.h"

World::World(void){

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

pVector World::reflect(pVector L, pVector N){
	pVector R = N - L * ((L*N) * 2);
	return R.normal;
}

void World::intersection(IntersectData &id, Point point, pVector normal, pVector incoming, pVector outgoing, vector<LightSource*> lights){
	id.point = point;
	id.normal = normal.normal;
	id.incoming = incoming.normal;
	id.reflective = reflect(id.incoming, id.normal);
	id.outgoing = outgoing.normal;
	id.lights = lights;
}

Light World::spawn(Ray ray){
	Light light = { 0 };
	float closest = sqrt(myMax) / 3;
	Point pClosest = maxPoint;
	for (int i = 0; i < objectList.size(); ++i){
		Point temp = objectList[i]->intersect(ray);
		float distance = temp.distance(ray.start);
		cout << "Distance: " << distance << endl;
		if (distance < closest){
			IntersectData id;
			this->intersection(id, temp, objectList[i]->normal(temp), ray.direction, {ray.start - temp}, lightList);
			light = objectList[i]->material->illuminate(id);
			closest = distance;
		}
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
 