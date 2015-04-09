#include "Phong.h"

Phong::Phong(Color color){
	ka = 0.0;
	kd = 0.8;
	ks = 0.0;
	ke = 1.0;
	c = color;
	specular = white;
}

Phong::Phong(){
	ka = 0.0;
	kd = 1000.0;
	ks = 1.0;
	ke = 0.0;
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
	pVector S = id.incoming;
	pVector N = id.normal;
	pVector R = id.reflective;
	pVector V = id.outgoing;
	float ambient;
	float specular = 0.0;
	float diffuse = 0.0;
	for (int l = 0; l < id.lights.size(); ++l){
		S = { id.lights[l]->position - id.point };
		S = S.normal;
		V = {id.point - id.lights[l]->position};
		V = V.normal;
		R = reflect(S, N);
		Color s = (white * id.lights[l]->color) * (R*V);
		cdiff = cdiff + ((c * id.lights[l]->color) *  (S*N) * kd);
		cspec = cspec + (s.power(ke) * ks);
		diffuse += id.lights[l]->light.irradiance * (S * N) * kd;
		specular += pow(id.lights[l]->light.irradiance * (R*V), ke) * ks;
	}
	L.irradiance = diffuse + specular;
	return (cdiff + cspec);
}