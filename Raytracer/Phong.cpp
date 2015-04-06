#include "Phong.h"

Phong::Phong(){
	ka = 0.0;
	kd = 1.0;
	ks = 1.0;
}

Phong::Phong(float a, float d, float s, float e){
	ka = a;
	kd = d;
	ks = s;
	ke = e;
}

Phong::~Phong(){

}

Light Phong::illuminate(IntersectData id){
	Light L = {0.0};
	pVector S = id.incoming;
	pVector N = id.normal;
	pVector R = id.reflective;
	pVector V = id.outgoing;
	float ambient;
	float specular = 0.0;
	float diffuse = 0.0;
	for (int l = 0; l < id.lights.size(); ++l){
		diffuse += id.lights[l]->light.irradiance * (S * N) * kd;
		specular += pow(id.lights[l]->light.irradiance * (R*V), ke) * ks;
	}
	L.irradiance = diffuse + specular;
	return L;
}