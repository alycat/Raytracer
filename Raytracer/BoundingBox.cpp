#include "BoundingBox.h"


BoundingBox::BoundingBox(){
}

BoundingBox::~BoundingBox(){

}

bool BoundingBox::hit(Ray ray){
	return false;
}

int BoundingBox::longestAxis(){
	Point p = origin;
	for (int i = 0; i < 8; ++i){
		Point t = {abs(corners[i].x), abs(corners[i].y), abs(corners[i].z) };
		p = p + t;
	}
	int value = (p.x > p.y) ? (p.x > p.z ? 0 : 2) : (p.y > p.z ? 1 : 2);
	return value;
}