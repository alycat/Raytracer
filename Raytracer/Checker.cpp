#include "Checker.h"

Checker::Checker(UV uv){
	a = yellow;
	b = red;
	u_v = uv;
	hor = 12;
	vert = 33;
}

Checker::~Checker(void){

}

Checker::Checker(UV uv, Color c){
	a = c;
	b = white - a;
	u_v = uv;
	hor = 25;
	vert = 10;
}

Checker::Checker(UV uv, Color c1, Color c2){
	a = c1;
	b = c2;
	u_v = uv;
	hor = 20;
	vert = 20;
}

Light Checker::illuminate(IntersectData id){
	float u = 0.0, v = 0.0;
	Color c = b;
	u = (id.point.x - u_v.left) / (u_v.right - u_v.left);
	v = (id.point.z - u_v.up) / (u_v.up - u_v.down);
	bool row = false, col = false;
	if (((int)(u * hor) % hor) % 2 == 0){
		row = true;
	}
	if (((int)(v * vert) % vert) % 2 == 0){
		col = true;
	}
	if (row == col){
		c = a;
	}
	c = (c * id.light->color) * (id.source * id.normal) * 0.005;
	return{ c};
}