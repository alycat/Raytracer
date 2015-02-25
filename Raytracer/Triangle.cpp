#include "Triangle.h"

Triangle::Triangle(void){
	p1 = { 0, 1, 1.5 }, p2 = {-1, -1, 1.5}, p3 = {1, -1, 1.5};
}

Triangle::~Triangle(void){

}

Triangle::Triangle(Point a, Point b, Point c){
	p1 = a, p2 = b, p3 = c;
}

Point Triangle::intersect(Ray ray){
	Point p = maxPoint;
	pVector AB = { p2 };
	AB = AB - p1;
	pVector AC = { p3 };
	AC = AC - p1;
	pVector N = AB;
	N.cross(AC);
	float A = N.v.x, B = N.v.y, C = N.v.z;
	float F = -(A*p1.x + B*p1.y + C*p1.z);
	float denominator = ray.direction.v.x * A + ray.direction.v.y * B + ray.direction.v.z * C;
	if (denominator != 0){
		float numerator = -(A * ray.start.x + B * ray.start.y + C * ray.start.z + F);
		float omega = numerator / denominator;
		if (omega > 0){
			Point temp = { ray.start.x + ray.direction.v.x * omega, ray.start.y + ray.direction.v.y * omega, ray.start.z + ray.direction.v.z * omega };
			pVector TA = { temp }, TB = { temp }, TC = { temp };
			TA = TA - p1;
			TB = TB - p2;
			TC = TC - p3;
			float sum = acos(TA.dot(TB) / (TA.mag * TB.mag)) + acos(TB.dot(TC) / (TB.mag * TC.mag)) + acos(TC.dot(TA) / (TC.mag * TA.mag));
			if (sum >= 2 * 3.1415){
				p = temp;
			}
		}
	}
	return p;
}

void Triangle::transform(Matrix matrix){

}