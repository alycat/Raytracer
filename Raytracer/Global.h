#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <cmath>
#include <Windows.h>
#include <limits>
#include "cmatrix"

#define M_PI 3.141592653589793238462643383279502884 /*PI*/

typedef techsoft::matrix<float> Matrix;
typedef std::complex<float> fComplex;
typedef techsoft::matrix<fComplex> CMatrix;
typedef std::valarray<float> Vector;

const float myMax= (std::numeric_limits<float>::max)();

/*Matrix Scale(Point s){
	Matrix scale = Matrix(4, 4);
	scale[0][0] = s.x, scale[1][1] = s.y, scale[2][2] = s.z, scale[3][3] = 1.0;
	return scale;
}*/

struct Point{
	float x;
	float y;
	float z;

	float distance(Point p){
		return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2));
	}

	void transform(Point p){
		x += p.x;
		y += p.y;
		z += p.z;
	}
};



const Point origin = { 0, 0, 0 };
const Point maxPoint = { myMax, myMax, myMax };

struct pVector{
	Point v;

	pVector operator*(const float& in){
		return{ {v.x * in, v.y * in, v.z * in} };
	}

	pVector operator-(const pVector& in){
		return{ { v.x - in.v.x, v.y - in.v.y, v.z - in.v.z } };
	}

	pVector operator-(const Point& in){
		return{ { v.x - in.x, v.y - in.y, v.z - in.z } };
	}

	pVector operator+(const pVector& in){
		return{ { v.x + in.v.x, v.y + in.v.y, v.z + in.v.z } };
	}

	void cross(pVector pv){
		v = { (v.y * pv.v.z) - (v.z * pv.v.y), (v.z * pv.v.x) - (v.x * pv.v.z), (v.x * pv.v.y) - (v.y * pv.v.x) };
	}

	float dot(pVector pv){
		return (v.x * pv.v.x) + (v.y * pv.v.y) + (v.z * pv.v.z);
	}

	float length(){
		return v.distance(origin);
	}

	_declspec(property(get = length)) float mag;

	void normalize(){
		float u = length();
		v.x /= u;
		v.y /= u;
		v.z /= u;
	}

	void transform(pVector pv){
		v.x += pv.v.x;
		v.y += pv.v.y;
		v.z += pv.v.z;
	}


	void transform(Point p){
		v.x += p.x;
		v.y += p.y;
		v.z += p.z;
	}

	void scalar(float s){
		v = { v.x *= s, v.y *= s, v.z *= s };
	}
};

struct Ray{
	Point start;
	pVector direction;
};

struct Color{
	int r;
	int g;
	int b;

	COLORREF getColorRef(){
		return RGB(r, g, b);
	}

	_declspec(property(get = getColorRef)) COLORREF getColor;
};

static Color background = {100, 149, 237}; //cornflower blue
static Color white = {255, 255, 255};
static Color black = { 0, 0, 0 };
static Color red = { 255, 0, 0 };
static Color green = { 0, 255, 0 };
static Color blue = { 0, 0, 255 };

#endif