#include "Triangle.h"

Triangle::Triangle(void){
	p1 = { 0, 10000, 1000 }, p2 = { -10000, 0, 1000 }, p3 = { 10000, 0, 1000 };
	vM = Matrix(4, 4);
	vM.unit();
	pVector v1 = { p2 - p1 };
	pVector v2 = { p3 - p1 };
	vn = v1.getCross(v2);
	vn = vn.normal;
	material = new Phong(red);
}

Triangle::~Triangle(void){

}

Triangle::Triangle(Point a, Point b, Point c){
	p1 = a, p2 = b, p3 = c;
	vM = Matrix(4, 4);
	vM.unit();
	pVector v1 = { p1 - p2 };
	pVector v2 = { p1 - p3 };
	vn = v1.getCross(v2);
	vn = vn.normal;
	material = new Phong(red);
}

/*
 Moller-Trumbore intersection algorithm
 Param: Ray, 
 Returns closest point of intersection
*/
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
	vM = matrix * vM;
	//vM = vM * matrix;
	Point trans = {vM[0][3], vM[1][3], vM[2][3]};
	trans = {vM[3][0], vM[3][1], vM[3][2]};
	//trans = origin;
	p1 + trans;
	p2 + trans;
	p3 + trans;
	/*Matrix posM1 = Matrix(1, 4);
	Matrix posM2 = Matrix(1, 4);
	Matrix posM3 = Matrix(1, 4);
	posM1[0][0] = p1.x, posM1[0][1] = p1.y, posM1[0][2] = p1.z, posM1[0][3] = 1.0;
	posM2[0][0] = p2.x, posM2[0][1] = p2.y, posM2[0][2] = p2.z, posM2[0][3] = 1.0;
	posM3[0][0] = p3.x, posM3[0][1] = p3.y, posM3[0][2] = p3.z, posM3[0][3] = 1.0;
	posM1 = posM1 * matrix * vM;
	posM2 = posM2 * matrix * vM;
	posM3 = posM3 * matrix * vM;
	p1 = { posM1[0][0], posM1[0][1], posM1[0][2] };
	p2 = { posM2[0][0], posM2[0][1], posM2[0][2] };
	p3 = { posM3[0][0], posM3[0][1], posM3[0][2] };*/
}

pVector Triangle::normal(Point intersect){
	return vn;
}