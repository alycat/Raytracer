#include "Triangle.h"

Triangle::Triangle(void){
	p1 = { 0, 1, 15 }, p2 = {-1, -1, 15}, p3 = {1, -1, 15};
	vM = Matrix(4, 4);
	vM.unit();
}

Triangle::~Triangle(void){

}

Triangle::Triangle(Point a, Point b, Point c){
	p1 = a, p2 = b, p3 = c;
	vM = Matrix(4, 4);
	vM.unit();
}

Point Triangle::intersect(Ray ray){
	pVector e_1 = { p2 - p1 };
	pVector e_2 = { p3 - p1 };
	pVector P = ray.direction.getCross(e_2);
	float det = e_1*P;

	if (det > -EPSILON && det < EPSILON){
		return maxPoint;
	}

	float f = 1.0f / det;

	pVector T = { ray.start - p1};
	float u = (T*P) * f;
	if (u < 0.0f || u > 1.0f){
		return maxPoint;
	}

	pVector Q = T.getCross(e_1);

	float v = (Q*ray.direction) * f;
	if (v < 0 || u + v > 1){
		return maxPoint;
	}

	float t = (Q*e_2)* f;

	if (t <= EPSILON){
		return maxPoint;
	}
	return{ u, v, t };
}

void Triangle::transform(Matrix matrix){
	/*vM = matrix * vM;
	//vM = vM * matrix;
	Point trans = {vM[0][3], vM[1][3], vM[2][3]};
	//trans = {vM[3][0], vM[3][1], vM[3][2]};
	trans = origin;
	p1 + trans;
	p2 + trans;
	p3 + trans;*/
}