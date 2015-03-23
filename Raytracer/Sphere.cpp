#include "Sphere.h"

Sphere::Sphere(void){
	Sphere(origin, 1);
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::~Sphere(void){

}

Sphere::Sphere(Color c) :Object(c){
	Sphere();
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::Sphere(Point cen, float r){
	center = cen;
	radius = r;
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::Sphere(Point cen, float r, Color c) :Object(c){
	center = cen;
	radius = r;
	vM = Matrix(4, 4);
	vM.unit();
}

Point Sphere::intersect(Ray ray){
	pVector v = { ray.start - center };
	float b = 2 * (ray.direction * v);
	float c = (v*v) - (radius * radius);
	float disc = (b*b) - (4 * c);
	if (disc < 0){
		return maxPoint;
	}

	float t_1 = (-b + sqrt(disc))/2;
	float t_2 = (-b - sqrt(disc))/2;
	float t = (t_1 < t_2) && t_1 > 0 ? t_1 : t_2;
	if (t >= 0){
		return (ray.direction * t).v + ray.start;
	}
	return maxPoint;

}

void Sphere::transform(Matrix matrix){
	vM = Translate(center) * vM;
	vM = matrix * vM;
	center = { origin.x + vM[0][3], origin.y + vM[1][3], origin.z + vM[2][3] };
}