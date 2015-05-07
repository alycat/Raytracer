#include "World.h"
#include <ppl.h>

using namespace Concurrency;

World::World(void){
	tree = new KDNode();
}

World::~World(void){
	/*for (int i = 0; i < objectList.size(); ++i){
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
	tree = nullptr;*/
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
	box.box = { -100, 100, 100, -100, 100, -100 };
	tree->box = box;
	for (int i = 0; i < objectList.size(); ++i){
		tree->objects.push_back(objectList[i]);
	}
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
				if (nearest[n]->box.hit(ray)){
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

							//if (intersection(shadow, i, nearest[n]->objects[i]))
							//{
							light = light + nearest[n]->objects[i]->material->illuminate(id);
							pVector I = ray.direction.normal;

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
									Point c = dynamic_cast<Sphere*>(nearest[n]->objects[i])->center;
									c = { 0, 0, c.z };
									Point p = temp - c;
									Ray transray = { temp, transmit(I, N) };
									Light t = spawn(transray, depth + 1);
									t.irradiance = t.irradiance * nearest[n]->objects[i]->k_t;
									light = light + t;
								}
							}
							//}



						});
						closest = distance;
					}
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
	float original = pClosest.distance(ray.start);
	vector<KDNode*> nearest = tree->getNodes(tree, ray);
	//nearest = tree->traverse(tree);
	parallel_for(int(0), (int)nearest.size(), [&](int n)
	{
		parallel_for(int(0), (int)nearest[n]->objects.size(), [&](int i){
			if (nearest[n]->box.hit(ray)){
				Point temp = nearest[n]->objects[i]->intersect(ray);
				float dis = temp.distance(ray.start);
				if (dis < closest && original - dis > 0.1){
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