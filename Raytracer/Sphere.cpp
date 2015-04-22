#include "Sphere.h"

Sphere::Sphere(void){
	center = origin;
	radius = 1;
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
	material = new Phong();
}

Sphere::~Sphere(void){
	if (material){
		delete material;
	}
	material = nullptr;
}

Sphere::Sphere(Color c){
	center = origin;
	color = c;
	vM = Matrix(4, 4);
	vM.unit();
	material = new Phong(c);
}

Sphere::Sphere(Point cen, float r){
	center = cen;
	radius = r;
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
	material = new Phong();
}

Sphere::Sphere(Point cen, float r, Color c){
	center = cen;
	radius = r;
	color = c;
	vM = Matrix(4, 4);
	vM.unit();
	material = new Phong(c);
}

Point Sphere::intersect(Ray ray){
	pVector v = { ray.start - center };
	float a = ray.direction * ray.direction;
	float b = (ray.direction * v) * 2;
	float c = (v*v) - (radius * radius);
	float disc = (b*b) - (4*c);
	if (disc < 0){
		return maxPoint;
	}
	float t_1 = (-b + sqrt(disc))/(2);
	float t_2 = (-b - sqrt(disc))/(2);
	float t = (t_1 < t_2) ? t_1 : t_2;
	if (t > 0){
		return (ray.direction * t).v + ray.start;
	}
	return maxPoint;

}

void Sphere::transform(Matrix matrix){
	Matrix posM = Matrix(1, 4);
	posM[0][0] = center.x, posM[0][1] = center.y, posM[0][2] = center.z, posM[0][3] = 1.0;
	posM = posM * (matrix * vM);
	//center = {posM[0][0], posM[0][1], posM[0][2]};
}

pVector Sphere::normal(Point intersection){
	pVector n = { intersection - center };
	return n.normal;
}

Point Sphere::getMidPoint(){
	return center;
}

BoundingBox Sphere::getBBox(){
	BoundingBox box = BoundingBox();
	box.box	= {center.x - radius, center.x + radius, center.y + radius, center.y - radius, center.z + radius, center.z - radius};
	return box;
}

bool Sphere::hit(Ray ray){
	return false;
}