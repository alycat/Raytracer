#include "Rayleigh.h"

Rayleigh::Rayleigh(){
	c = white;
	kd = 0.001;
	ks = 0.3;
	ke = 1;
	n = 1.05;
	angle = -M_PI/5;
}

Rayleigh::~Rayleigh(){

}

Light Rayleigh::illuminate(IntersectData id){
	Light L = { 0.0 };
	
	Color s = (id.light->color) * (id.reflect * id.outgoing);
	Color cdiff = ((c * id.light->color) *  (id.source*id.normal) * kd);
	Color cspec = (s.power(ke) * ks);
	cdiff.forcePositive();
	cspec.forcePositive();
	L.irradiance = cdiff + cspec;
	pVector light_vector = {id.center - id.light->position};
	//light_vector.v.z = 0;
	float theta = light_vector.normal * id.normal;
	theta = 0;
	float cosine = theta;
	float cos_color = abs(cosine);
	cos_color *= cos_color;
	L.irradiance.r *= (5/(abs(2 - ((cos_color + 0.1)/RED))));
	L.irradiance.g *= (5/(abs(2 - ((cos_color + 0.1)/ GREEN))));
	L.irradiance.b *= (5/(abs(2 - ((cos_color + 0.1)/ BLUE))));
	return L;
}