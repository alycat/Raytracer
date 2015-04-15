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
	~BoundingBox();
	int longestAxis();
	bool hit(Ray ray);
};
#endif