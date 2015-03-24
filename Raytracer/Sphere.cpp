#include "Sphere.h"

Sphere::Sphere(void){
	center = origin;
	radius = 1;
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::~Sphere(void){

}

Sphere::Sphere(Color c){
	center = origin;
	color = c;
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::Sphere(Point cen, float r){
	center = cen;
	radius = r;
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
}

Sphere::Sphere(Point cen, float r, Color c){
	center = cen;
	radius = r;
	color = c;
	vM = Matrix(4, 4);
	vM.unit();
}

Point Sphere::intersect(Ray ray){
	pVector v = { ray.start - center };
	float a = ray.direction * ray.direction;
	float b = (ray.direction * v) * 2;
	float c = (v*v) - (radius * radius);
	float disc = (b*b) - (4 * a* c);
	if (disc < 0){
		return maxPoint;
	}
	float t_1 = (-b + sqrt(disc))/(2*a);
	float t_2 = (-b - sqrt(disc))/(2*a);
	float t = (t_1 < t_2) ? t_1 : t_2;
	if (t > 0){
		return (ray.direction * t).v + ray.start;
	}
	return maxPoint;

}

void Sphere::transform(Matrix matrix){
	/*vM = Translate(center) * vM;
	vM = matrix * vM;
	center = { origin.x + vM[0][3], origin.y + vM[1][3], origin.z + vM[2][3] };*/
}