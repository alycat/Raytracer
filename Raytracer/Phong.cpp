#include "Phong.h"

Phong::Phong(Color color){
	ka = 0.0;
	kd = 0.0009;
	ks = 0.5;
	ke = 0.9;
	c = color;
	specular = { 500, 500, 500 };
	specular = white;
}

Phong::Phong(){
	ka = 0.0;
	kd = 1000.0;
	ks = 2.0;
	ke = 10.0;
	c = blue;
	specular = white;
}

Phong::Phong(float a, float d, float s, float e){
	ka = a;
	kd = d;
	ks = s;
	ke = e;
	specular = white;
}

Phong::~Phong(){

}

Light Phong::illuminate(IntersectData id){
	Light L = {0.0};
	Color s = (id.light->color) * (id.reflect * id.outgoing);
	Color cdiff = ((c * id.light->color) *  (id.source*id.normal) * kd);
	Color cspec = (s.power(ke) * ks);
	//cdiff.forcePositive();
	//cspec.forcePositive();
	L.irradiance = cdiff + cspec;
	//L.irradiance.cap(L_max, 0.0f);
	return L;
}