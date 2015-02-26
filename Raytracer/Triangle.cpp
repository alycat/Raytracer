#include "Triangle.h"

Triangle::Triangle(void){
	p1 = { 0, 1, 1.5 }, p2 = {-1, -1, 1.5}, p3 = {1, -1, 1.5};
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
	ray.direction.normalize();
	Point p = maxPoint;
	Point ab = p2 - p1;
	Point ac = p3 - p1;
	pVector AB = { ab };
	pVector AC = { ac };
	pVector N = AB.getCross(AC);
	N.normalize();
	float denominator = (ray.direction.v.x * N.v.x) + (ray.direction.v.y * N.v.y) + (ray.direction.v.z * N.v.z);
	if (denominator != 0){
		float F = (N.v.x*p1.x) + (N.v.y*p1.y) + (N.v.z*p1.z);
		float numerator = -((N.v.x* ray.start.x) + (N.v.y * ray.start.y) + (N.v.z * ray.start.z) - F);
		float omega = numerator / denominator;
		if (omega > 0){
			Point temp = { ray.start.x + (ray.direction.v.x * omega), ray.start.y + (ray.direction.v.y * omega), ray.start.z + (ray.direction.v.z * omega) };
			Point ta = temp - p1, tb = temp - p2, tc = temp - p3;
			pVector TA = { ta }, TB = { tb }, TC = { tc };
			TA.normalize(), TB.normalize(), TC.normalize();
			float sum = acos(TA.dot(TB) / (TA.mag * TB.mag)) + acos(TB.dot(TC) / (TB.mag * TC.mag)) + acos(TC.dot(TA) / (TC.mag * TA.mag));
			if (sum >= 2 * M_PI){
				p = temp;
			}
		}
	}
	return p;
}

void Triangle::transform(Matrix matrix){
	vM = matrix * vM;
	Point trans = {vM[0][3], vM[1][3], vM[2][3]};
	p1 + trans;
	p2 + trans;
	p3 + trans;
}