#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <cmath>
#include <Windows.h>
#include <limits>
#include "cmatrix"
#include <vector>

#define M_PI 3.1415 /*PI*/
#define RED .152587890625
#define GREEN .075969140625
#define BLUE 0.04100625
/*#define RED 0.62
#define GREEN 0.525
#define BLUE 0.42*/
#define EPSILON 0.000001
#define new new(1, __FILE__, __LINE__)

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
		float nx = x - p.x, ny = y - p.y, nz = z - p.z;
		return sqrt((nx*nx) + (ny * ny) + (nz * nz));
	}

	float distance2(Point p){
		float nx = x - p.x, ny = y - p.y, nz = z - p.z;
		return ((nx*nx) + (ny * ny) + (nz * nz));
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

	Point operator*(const Point& in){
		return{ x*in.x, y * in.y, z*in.z };
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

	void abs(){
		x = std::abs(x);
		y = std::abs(y);
		z = std::abs(z);
	}
	
	Point absolute(){
		return{ std::abs(x), std::abs(y), std::abs(z) };
	}
};



const Point origin = { 0, 0, 0 };
const Point maxPoint = { myMax, myMax, myMax };
const float infinity = std::numeric_limits<float>::infinity();
const Point posLim = { infinity, infinity, infinity };
const Point negLim = { -infinity, -infinity, -infinity };

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

static const float L_max = 5000;
static const float L_dmax = 255;

struct Color{
	int r;
	int g;
	int b;

	void cap(float maxV, float minV){
		r = max(min((float)r, maxV), minV);
		g = max(min((float)g, maxV), minV);
		b = max(min((float)b, maxV), minV);
	}

	void forcePositive(){
		r = std::max(r, 0);
		g = std::max(g, 0);
		b = std::max(b, 0);
	}

	COLORREF ward(float L_average){
		float L = L_average;

		r *= L_max/L_dmax;
		g *= L_max/L_dmax;
		b *= L_max/L_dmax;

		float sf =(1.219f + pow((L_dmax / 2), 0.4f)) / (1.219f + pow(L, 0.4f));
		sf = pow(sf, 2.5f);
		sf /= L_dmax;
		r = (r *sf);
		g = (g *sf);
		b = (b *sf);

		r = min(max((float)r, 0.0f), L_dmax);
		g = min(max((float)g, 0.0f), L_dmax);
		b = min(max((float)b, 0.0f), L_dmax);
		return RGB(r, g, b);
	}

	COLORREF reinhard(float L_average){
		float a = 0.18;
		
		r *= L_max / L_dmax;
		g *= L_max / L_dmax;
		b *= L_max / L_dmax;
		
		float L = L_average;
		float f = a / L; 
		float r_s = f * r;
		float g_s = f * g;
		float b_s = f * b;

		float r_r = r_s / (1 + r_s);
		float g_r = g_s / (1 + g_s);
		float b_r = b_s / (1 + b_s);
		
		r_r = std::max(std::min(r_r, 1.0f), 0.0f);
		g_r = std::max(std::min(g_r, 1.0f), 0.0f);
		b_r = std::max(std::min(b_r, 1.0f), 0.0f);
		
		r = (r_r * L_dmax);
		g = (g_r * L_dmax);
		b = (b_r * L_dmax);

		return RGB(r, g, b);
	}

	COLORREF getColorRef(){
		r = std::max(std::min(r, 255), 0);
		g = std::max(std::min(g, 255), 0);
		b = std::max(std::min(b, 255), 0);

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

	Color operator/(const Color& in){
		return{ r / in.r, g / in.g, b / in.b };
	}

	Color operator*(const float& in){
		return{r* in, g* in, b * in};
	}

	Color power(float e){
		return{ pow(r, e), pow(g, e), pow(b, e) };
	}
};

static Color background = {100, 149, 237}; //cornflower blue
static Color cornflowerblue = { 100, 149, 237 };
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

struct BackgroundColor{
	Color background_color;

	BackgroundColor(){
		background_color = cornflowerblue;
	}

	void setBackgroundColor(Color color){
		background_color = color;
	}

	Color getBackgroundColor(){
		return background_color;
	}

	_declspec(property(get = getBackgroundColor, put = setBackgroundColor)) Color backgroundColor;
};

static BackgroundColor backColor;

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

struct Photon{
	Point position;
	float p_red;
	float p_green;
	float p_blue;
	float phi, theta;
	int flag;
};

struct Light{
	Color irradiance;

	Light operator+(const Light& in){
		return{irradiance + in.irradiance};
	}

	Light operator*(const float &in){
		return{ irradiance * in };
	}

	Light operator*(const Light& in){
		return{ irradiance * in.irradiance };
	}
};

//Light Source information struct
struct LightSource{
	Point position;
	Color color;
	Light light;
	int numPhotons;

	void emitPhotonsFromDiffuseLight(){
		int emitted = 0;
		float x = 1, y = 1, z = 1;
		while (emitted < numPhotons){
			do{
				x = rand() % 2 + (-1);
				y = rand() % 2 + (-1);
				z = rand() % 2 + (-1);
			} while (((x*x) + (y*y) + (z*z)) > 1);
			pVector d = { x, y, z };
			d = d.normal;
			Point p = position; 
			Ray ray = { p, d };
			emitted++;
		}
	}
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
	Point center;
};

static pVector reflect(pVector L, pVector N){
	pVector R = N - L * ((L*N) * 2);
	return R.normal;
}

static float FindMedian(vector<float>& f){
	size_t n = f.size() > 1 ? f.size() / 2 : 0;
	nth_element(f.begin(), f.begin() + n, f.end());
	return f[n];
}

static pVector transmit(pVector I, pVector N){
	float inc = 1.05;
	float dot = I*N;
	if (dot <= 0){
		N = N*-1;
		dot = N*I;
		//inc = 1 / inc;
	}
	
	float value = 1 - ((inc*inc)*(1 - (dot)*(dot)));
	value = value >= 0.0 ? sqrt(value) : 0.0;
	//value = value ? sqrt(1 - ((inc*inc)*(1 - (I*N)*(I*N)))) : 1.0;
	pVector T = value > 0 ? (I - N *(dot))*inc + N*value : I;
	
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



static float cosineTheta(float x){
	return ((pow((-4 * x + 4 * (4 + (x * x))), 1 / 3)) / 2) - (2 / pow((-4 * x + 4 * (4 + (x * x))), 1 / 3));
}

static Color cosineThetaColor(Color c){
	return{ cosineTheta(c.r), cosineTheta(c.g), cosineTheta(c.b) };
}

static Light cosineThetaLight(Light l){
	return{ cosineThetaColor(l.irradiance) };
}

static Color wavelength = { 0.62, .525, .42 };
#endif