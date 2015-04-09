#ifndef _ILLUMINATIONMODEL_H
#define _ILLUMINATIONMODEL_H

#include "Global.h"

/*"Abstract" class Illumination Model- All Illumination Models should inheret this class*/
class IlluminationModel{
public:
	Color c;
	IlluminationModel(void){};
	~IlluminationModel(void){};
	virtual Color illuminate(IntersectData id)=0;
};

#endif