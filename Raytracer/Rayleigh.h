#ifndef _RAYLEIGH_H
#define _RAYLEIGH_H
#include "IlluminationModel.h"

class Rayleigh :public IlluminationModel{
public:
	float n;
	float angle;
	Rayleigh();
	~Rayleigh();
	Light illuminate(IntersectData id);
};
#endif