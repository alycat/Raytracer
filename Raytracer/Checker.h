#ifndef _CHECKER_H
#define _CHECKER_H

#include "IlluminationModel.h"

class Checker : public IlluminationModel{
public:
	Checker(UV uv);
	Checker(UV uv, Color c);
	Checker(UV uv, Color c1, Color c2);
	~Checker(void);
	Light illuminate(IntersectData id) override;
protected:
	Color a, b;
	int hor, vert;
	UV u_v;
};
#endif