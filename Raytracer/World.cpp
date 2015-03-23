#include "World.h"

World::World(void){

}

World::~World(void){
	for (int i = 0; i < objectList.size(); ++i){
		delete objectList[i];
	}
	objectList.clear();
}

void World::add(Object* obj){
	objectList.push_back(obj);
}

void World::transform(Object obj){

}

void World::transformAllObjects(Matrix matrix){
	for (int i = 0; i < objectList.size(); ++i){
		objectList[i]->transform(matrix);
	}
}

void World::spawn(Ray ray){
	Point p = maxPoint;
	
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
 