#include "Phong.h"

Phong::Phong(Color color){
	ka = 0.0;
	kd = 0.005;
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
	Color cdiff = black, cspec = { 0, 0, 0 };
	Light L = {0.0};
	
	float ambient;
	//Color specular = {500, 500, 500};
	Color diffuse = black;
	pVector S = id.source;
	pVector V = id.outgoing;
	pVector R = id.reflect;
	pVector N = id.normal;
	Color s = (id.light->color) * (R*V);
	cdiff = ((c * id.light->color) *  (S*N) * kd);
	cspec = (s.power(ke) * ks);
	//diffuse = id.light->light.irradiance * (S * N) * kd;
	//specular = (id.light->light.irradiance*(R*V)).power(ke) * ks;
	L.irradiance = cdiff + cspec;
	//return (cdiff + cspec) * (id.light->light.irradiance / m_irr);
	return L;
}