#include "BoundingBox.h"


BoundingBox::BoundingBox(){
	box = {0, 0, 0, 0, 0, 0};
}

BoundingBox::~BoundingBox(){

}
Point BoundingBox::intersect(Ray ray, int type){
	Point fraction = { 1.0f, 1.0f, 1.0f };
	fraction = fraction / ray.direction.v;
	float t_minx = (box.left - ray.start.x) * fraction.x;
	float t_maxx = (box.right - ray.start.x) * fraction.x;
	float t_miny = (box.bottom - ray.start.y) * fraction.y;
	float t_maxy = (box.top - ray.start.y) * fraction.y;
	float t_minz = (box.front - ray.start.z) * fraction.z;
	float t_maxz = (box.back - ray.start.z) * fraction.z;

	float tmin = max(max(min(t_minx, t_maxx), min(t_miny, t_maxy)), min(t_minz, t_maxz));
	float tmax = min(min(max(t_minx, t_maxx), max(t_miny, t_maxy)), max(t_minz, t_maxz));

	if (tmax < 0){
		return maxPoint;
	}

	if (tmin > tmax){
		return maxPoint;
	}
	if (type == 0){
		return ray.start + (ray.direction.v * tmin);
	}
	return ray.start + (ray.direction.v * tmax);;
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
	float t_minz = (box.front - ray.start.z) * fraction.z;
	float t_maxz = (box.back - ray.start.z) * fraction.z;

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
	float z = abs(box.back - box.front);

	int value = (x > y) ? (x > z? 0 : 2) : (y > z? 1 : 2);
	return value;
}

void BoundingBox::expand(BoundingBox b){
	if (box.back > b.box.back){
		box.back = b.box.back;
	}
	if (box.front < b.box.front){
		box.front = b.box.front;
	}
	if (box.left > b.box.left){
		box.left = b.box.left;
	}
	if (box.right < b.box.right){
		box.right = b.box.right;
	}
	if (box.top < b.box.top){
		box.top = b.box.top;
	}
	if (box.bottom > b.box.bottom){
		box.bottom = b.box.bottom;
	}
}