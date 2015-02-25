#include "Sphere.h"

Sphere::Sphere(void){
	Sphere(origin, 1);
}

Sphere::~Sphere(void){

}

Sphere::Sphere(Color c) :Object(c){
	Sphere();
}

Sphere::Sphere(Point cen, float r){
	center = cen;
	radius = r;
}

Sphere::Sphere(Point cen, float r, Color c) :Object(c){
	center = cen;
	radius = r;
}

Point Sphere::intersect(Ray ray){
	Point p = maxPoint;
	float C = pow((ray.start.x - center.x), 2) + pow((ray.start.y - center.y), 2) + pow((ray.start.z - center.z), 2) - pow(radius, 2);
	float B = 2 * ((ray.direction.v.x * (ray.start.x - center.x)) + (ray.direction.v.y * (ray.start.y - center.y)) + (ray.direction.v.z * (ray.start.z - center.z)));
	float discriminant = pow(B, 2) - (4 * C);
	if (discriminant >= 0){
		float omega = (-B + sqrt(discriminant)) / 2 < (-B - sqrt(discriminant)) / 2 ? (-B + sqrt(discriminant)) / 2 : (-B - sqrt(discriminant)) / 2;
		pVector temp = ray.direction;
		temp.scalar(omega);
		temp.transform(ray.start);
		p = temp.v;
	}

	cout << "discriminant: " << discriminant << endl;
	return p;
}

void Sphere::transform(Matrix matrix){

}