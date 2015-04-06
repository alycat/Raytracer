#ifndef _PHONG_H
#define _PHONG_H

#include "IlluminationModel.h"

class Phong: public IlluminationModel{
public:
	Phong(void);
	Phong(float a, float d, float s, float e);
	~Phong(void);
	Light illuminate(IntersectData id) override;
protected:
	float ka, kd, ks; //ambient, diffuse, & specular constants
	float ke; //specular exponent
};

#endif