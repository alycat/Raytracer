#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include "Global.h"
class BoundingBox{
public:
	struct BBox{
		float left;
		float right;
		float top;
		float bottom;
		float back;
		float front;
	};

	BBox box;
	BoundingBox();
	void expand(BoundingBox b);
	~BoundingBox();
	int longestAxis();
	Point intersect(Ray ray);
	bool hit(Ray ray);
};
#endif