#include "Phong.h"

Phong::Phong(Color color){
	ka = 0.0;
	kd = 10.0;
	ks = 5.0;
	ke = 1.0;
	c = color;
	specular = white;
}

Phong::Phong(){
	ka = 0.0;
	kd = 1000.0;
	ks = 1.0;
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

Color Phong::illuminate(IntersectData id){
	Color cdiff = {0,0,0}, cspec = { 0, 0, 0 };
	Light L = {0.0};
	
	float ambient;
	float specular = 0.0;
	float diffuse = 0.0;
	pVector S = id.source;
	pVector V = id.outgoing;
	pVector R = id.reflect;
	pVector N = id.normal;
	Color s = (white * id.light->color) * (R*V);
	cdiff = cdiff + ((c * id.light->color * (id.light->light.irradiance / m_irr)) *  (S*N) * kd);
	cspec = cspec + (s.power(ke) * ks * (id.light->light.irradiance / m_irr));
	diffuse += id.light->light.irradiance * (S * N) * kd;
	specular += pow(id.light->light.irradiance * (R*V), ke) * ks;
	L.irradiance = diffuse + specular;
	return (cdiff + cspec);
}