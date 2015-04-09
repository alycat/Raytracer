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


void World::intersection(IntersectData &id, Point point, pVector normal, LightSource* light){
	id.point = point;
	id.normal = normal.normal;
	id.source = {light->position - point};
	id.source = id.source.normal;
	id.outgoing = { point - light->position };
	id.outgoing = id.outgoing.normal;
	id.reflect = reflect(id.source, id.normal);
	id.light = light;
}

Color World::spawn(Ray ray){
	Color color = background;
	float closest = sqrt(myMax) / 3;
	Point pClosest = maxPoint;
	for (int i = 0; i < objectList.size(); ++i){
		Point temp = objectList[i]->intersect(ray);
		float distance = temp.distance(ray.start);
		cout << "Distance: " << distance << endl;
		if (distance < closest){
			IntersectData id;
			color = black;
			for (int l = 0; l < lightList.size(); ++l){
				this->intersection(id, temp, objectList[i]->normal(temp), lightList[l]);
				Ray shadow = { temp, { lightList[l]->position -temp} };
				shadow.direction = shadow.direction.normal;
				if (!intersection(shadow)){
					color = color + objectList[i]->material->illuminate(id);
				}
				closest = distance;
			}
		}
	}
	return color;
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

bool World:: intersection(Ray ray){
	float closest = sqrt(myMax) / 3;
	Point pClosest = maxPoint;
	for (int i = 0; i < objectList.size(); ++i){
		Point temp = objectList[i]->intersect(ray);
		float distance = temp.distance(ray.start);
		if (distance < closest){
			return true;
		}
	}
	return false;
}