#include "BoundingBox.h"


BoundingBox::BoundingBox(){
	box = {-100, 100, 100, -100, -100, 100};
}

BoundingBox::~BoundingBox(){

}

/**
	AABB-ray collision test
	http://www.scratchapixel.com/old/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
**/
bool BoundingBox::hit(Ray ray){
	Point fraction = { 1.0f, 1.0f, 1.0f };
	fraction = fraction / ray.direction.v;
	float t_minx = (box.left - ray.start.x) * fraction.x;
	float t_maxx = (box.right - ray.start.x) * fraction.x;
	float t_miny = (box.bottom - ray.start.y) * fraction.y;
	float t_maxy = (box.top - ray.start.y) * fraction.y;
	float t_minz = (box.back - ray.start.z) * fraction.z;
	float t_maxz = (box.front - ray.start.z) * fraction.z;

	float tmin = max(max(min(t_minx, t_maxx), min(t_miny, t_maxy)), min(t_minz, t_maxz));
	float tmax = min(min(max(t_minx, t_maxx), max(t_miny, t_maxy)), max(t_minz, t_maxz));

	if (tmax < 0){
		return false;
	}

	if (tmin > tmax){
		return false;
	}
	return true;
}

int BoundingBox::longestAxis(){
	float x = abs(box.right - box.left);
	float y = abs(box.top - box.bottom);
	float z = abs(box.front - box.back);

	int value = (x > y) ? (x > z? 0 : 2) : (y > z? 1 : 2);
	return value;
}