#ifndef _PHONG_H
#define _PHONG_H

#include "IlluminationModel.h"

class Phong: public IlluminationModel{
public:
	Color specular;
	Phong(Color color);
	Phong(void);
	Phong(float a, float d, float s, float e);
	~Phong(void);
	Color illuminate(IntersectData id) override;
protected:
	float ka, kd, ks; //ambient, diffuse, & specular constants
	float ke; //specular exponent
};

#endif