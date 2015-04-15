#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include "Global.h"
class BoundingBox{
public:
	struct{
		Point corners[8];
	};
	BoundingBox();
	~BoundingBox();
	int longestAxis();
	bool hit(Ray ray);
};
#endif