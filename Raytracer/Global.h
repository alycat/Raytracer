#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <cmath>
#include <Windows.h>
#include <limits>
#include "cmatrix"
#include <vector>

#define M_PI 3.1415 /*PI*/
#define EPSILON 0.000001

using namespace std;

typedef techsoft::matrix<float> Matrix;
typedef std::complex<float> fComplex;
typedef techsoft::matrix<fComplex> CMatrix;
typedef std::valarray<float> Vector;

const float myMax= (std::numeric_limits<float>::max)();

const float max_depth = 5;

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

	Point operator+(const Point& in){
		return{ x + in.x, y + in.y, z + in.z };
	}

	Point operator-(const Point& in){
		return{ x - in.x, y - in.y, z - in.z };
	}

	Point operator*(const float& in){
		return{ x*in, y*in, z*in };
	}

	Point operator/(const float& in){
		return{ x / in, y / in, z / in };
	}

	Point operator/(const Point& in){
		return{x/in.x, y/in.y, z/in.z};
	}

	bool operator<(const Point& in){
		return ((x < in.x) && (y < in.y) && (z < in.z));
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

	pVector getCross(pVector pv){
		return{ { (v.y * pv.v.z) - (v.z * pv.v.y), (v.z * pv.v.x) - (v.x * pv.v.z), (v.x * pv.v.y) - (v.y * pv.v.x) } };
	}

	void cross(pVector pv){
		v = { (v.y * pv.v.z) - (v.z * pv.v.y), (v.z * pv.v.x) - (v.x * pv.v.z), (v.x * pv.v.y) - (v.y * pv.v.x) };
	}

	float dot(pVector pv){
		return ((v.x * pv.v.x) + (v.y * pv.v.y) + (v.z * pv.v.z));
	}

	float operator*(const pVector& in){
		return ((v.x * in.v.x) + (v.y * in.v.y) + (v.z * in.v.z));
	}

	float length(){
		return v.distance(origin);
	}

	_declspec(property(get = length)) float mag;

	void normalize(){
		float u = sqrt((v.x*v.x) + (v.y * v.y) + (v.z*v.z));
		v.x /= u;
		v.y /= u;
		v.z /= u;
	}

	pVector normalVector(){
		pVector p = {v};
		float u = sqrt(p*p);
		return{ { p.v.x / u, p.v.y / u, p.v.z / u } };
	}

	_declspec(property(get = normalVector)) pVector normal;

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

	void forcePositive(){
		r = std::max(r, 0);
		g = std::max(g, 0);
		b = std::max(b, 0);
	}

	COLORREF getColorRef(){
		if (r < 0){
			r = 0;
		}
		r = std::min(r, 255);
		g = std::min(g, 255);
		b = std::min(b, 255);
		return RGB(r, g, b);
	}

	_declspec(property(get = getColorRef)) COLORREF getColor;

	Color operator+(const Color& in){
		return{r + in.r, g + in.g, b + in.b};
	}

	Color operator-(const Color& in){
		return{ r - in.r, g - in.g, b - in.b };
	}

	Color operator*(const Color& in){
		return{r* in.r, g *in.g, b * in.b};
	}

	Color operator/(const float& in){
		return{ r / in, g / in, b / in };
	}

	Color operator*(const float& in){
		return{r* in, g* in, b * in};
	}

	Color power(float e){
		return{ pow(r, e), pow(g, e), pow(b, e) };
	}
};

static Color background = {100, 149, 237}; //cornflower blue
static Color white = {255, 255, 255};
static Color black = { 0, 0, 0 };
static Color red = { 255, 0, 0 };
static Color green = { 0, 255, 0 };
static Color blue = { 0, 0, 255 };
static Color fuchsia = { 255, 0, 255 };
static Color purple = {128, 0, 128};
static Color yellow = {255, 255, 0};
static Color grey = {127, 127, 127};
static Color orange = {255, 127, 0};
static float m_irr = 1.0f;

//both row & column major scale matrix
static Matrix Scale(Point s){
	Matrix scale = Matrix(4, 4);
	scale[0][0] = s.x, scale[0][1] = 0.0, scale[0][2] = 0.0, scale[0][3] = 0.0;
	scale[1][0] = 0.0, scale[1][1] = s.y, scale[1][2] = 0.0, scale[1][3] = 0.0;
	scale[2][0] = 0.0, scale[2][1] = 0.0, scale[2][2] = s.z, scale[2][3] = 0.0;
	scale[3][0] = 0.0, scale[3][1] = 0.0, scale[3][2] = 0.0, scale[3][3] = 1.0;
	return scale;
}

//row-major translation matrix
static Matrix Translate(Point p){
	Matrix trans = Matrix(4, 4);
	trans.unit();
	trans[3][0] = p.x, trans[3][1] = p.y, trans[3][2] = p.z;
	return trans;
}

//row-major rotation around x-axis matrix
static Matrix RotateX(float angle){
	Matrix rot = Matrix(4, 4);
	rot.unit();
	rot[1][1] = cos(angle), rot[1][2] = sin(angle);
	rot[2][1] = -sin(angle), rot[2][2] = cos(angle);
	return rot;
}

//row-major rotation around y-axis matrix
static Matrix RotateY(float angle){
	Matrix rot = Matrix(4, 4);
	rot.unit();
	rot[0][0] = cos(angle), rot[0][2] = -sin(angle);
	rot[0][1] = sin(angle), rot[2][2] = cos(angle);
	return rot;
}

//row=major rotation around z-axis matrix
static Matrix RotateZ(float angle){
	Matrix rot = Matrix(4, 4);
	rot.unit();
	rot[0][0] = cos(angle), rot[0][1] = sin(angle);
	rot[1][0] = -sin(angle), rot[1][1] = cos(angle);
	return rot;
}

struct Light{
	Color irradiance;

	Light operator+(const Light& in){
		return{irradiance + in.irradiance};
	}
};

//Light Source information struct
struct LightSource{
	Point position;
	Color color;
	Light light;
};

//struct for intersection of object with ray
struct IntersectData{
	Point point;
	pVector normal;
	pVector source;
	pVector outgoing;
	pVector reflect;
	LightSource* light;
	pVector camera;
};

static pVector reflect(pVector L, pVector N){
	pVector R = N - L * ((L*N) * 2);
	return R.normal;
}

static pVector transmit(pVector I, pVector N){
	pVector T = I - (N * 2.0) * (N * I);
	return T.normal;
}

struct UV{
	float left;
	float right;
	float down;
	float up;
};

struct ShadeRect{
	pVector Normal;
	Point intersection;
};
#endif