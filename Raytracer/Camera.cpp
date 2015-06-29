#include "Camera.h"
#include <ppl.h>

using namespace Concurrency;

Camera::Camera(){
	filmplane = { 400, 300, 40000, 30000, 50000 };
	position = { 0, 0.0, 0 };
	//position = { -1.2, 0.0, -5 };
	//position = origin;
	lookat = { { 0.0, 0.0, 1.0 } };
	up = { { 0.0, 1.0, 0.0 } };

	n = { position - lookat.v };
	n = n.normal;
	u = up.getCross(n).normal;
	v = n.getCross(u);

	viewMatrix = Matrix(4, 4);
	pVector eyeP = { position };
	viewMatrix[0][0] = u.v.x, viewMatrix[0][1] = u.v.y, viewMatrix[0][2] = u.v.z, viewMatrix[0][3] = u.dot(eyeP * -1);
	viewMatrix[1][0] = v.v.x, viewMatrix[1][1] = v.v.y, viewMatrix[1][2] = v.v.z, viewMatrix[1][3] = v.dot(eyeP * -1);
	viewMatrix[2][0] = n.v.x, viewMatrix[2][1] = n.v.y, viewMatrix[2][2] = n.v.z, viewMatrix[2][3] = n.dot(eyeP * -1);
	viewMatrix[3][0] = 0, viewMatrix[3][1] = 0, viewMatrix[3][2] = 0, viewMatrix[3][3] = 1;
	
}

Camera::~Camera(){

}

void Camera::render(World world, HDC hdc){
	pixelW = filmplane.w / filmplane.W;
	pixelH = filmplane.h / filmplane.H;
	Point top_left = {(-filmplane.w + pixelW)/2, (filmplane.h - pixelH)/2, filmplane.f};
	//top_left = {(filmplane.h - pixelW)/2, (-filmplane.h + pixelH)/2, filmplane.f};
	float sx = top_left.x;
	float px = sx;
	float py = top_left.y;
	pVector middleScreen = {filmplane.W/2, filmplane.H/2, filmplane.f};
	middleScreen = middleScreen.normal;
	//Point **pixels = Point[filmplane.H][filmplane.W];
	Point **pixels = new Point*[filmplane.H];
	Color **colors = new Color*[filmplane.H];
	int N = filmplane.H * filmplane.W;
	float logL = 0;
	parallel_for(int(0), (int)filmplane.H, [&](int y){
		int p_y = py - (pixelH*y);
		pixels[y] = new Point[filmplane.W];
		colors[y] = new Color[filmplane.W];
		parallel_for(int(0), (int)filmplane.W, [&](int x){
			Color color = background;
			int p_x = px + pixelW*x;
			Ray ray = { position, { p_x - position.x, p_y - position.y, filmplane.f - position.z } };
			ray.direction = ray.direction.normal;
			float angle = acosf(ray.direction * middleScreen);
			Light light = world.spawn(ray, 0);
			//Light light = { world.spawnPhoton(ray, 0).irradiance * world.spawn(ray, 0).irradiance};
		//	Light light = world.spawnPhoton(ray, 0);
			color = light.irradiance;
			//SetPixel(hdc, x, y, color.getColorRef());
			pixels[y][x] = { x, y, 0 };
			colors[y][x] = color;
			SetPixel(hdc,x, y, color.getColorRef());
		});
	});

	
	for (int i = 0; i < filmplane.W; ++i){
		for (int j = 0; j < filmplane.H; ++j){
			Point p = pixels[j][i];

			float colorL = 0.27*colors[j][i].r + 0.67*colors[j][i].g + 0.06*colors[j][i].b;
			//colorL = max(0.0f, min(colorL, L_max));
			logL += min(max(0.0f, (float)log(0.000001 + colorL)), L_max);
		}
	}
	logL /= N;
	logL = max(min(logL, L_max), 0.0f);

	for (int i = 0; i < filmplane.W; ++i){
		for (int j = 0; j < filmplane.H; ++j){
			Point p = pixels[j][i];
			SetPixel(hdc, p.x, p.y, colors[j][i].ward(logL));
		//	SetPixel(hdc, p.x, p.y, colors[j][i].getColorRef());
		}
	}

	parallel_for(int(0), (int)filmplane.H, [&](int y){
		delete[] pixels[y];
		delete[] colors[y];
	});

	delete[] pixels;
	delete[] colors;
}

void Camera::update(Point nP){
	position = nP;
	pVector zaxis = lookat - position;
	zaxis.normalize();
	pVector xaxis = up;
	xaxis.cross(zaxis);
	pVector yaxis = xaxis;
	yaxis.cross(zaxis * -1);

	viewMatrix[0][0] = xaxis.v.x, viewMatrix[0][1] = xaxis.v.y, viewMatrix[0][2] = xaxis.v.z;
	viewMatrix[1][0] = yaxis.v.x, viewMatrix[1][1] = yaxis.v.y, viewMatrix[1][2] = yaxis.v.z;
	viewMatrix[2][0] = zaxis.v.x, viewMatrix[2][1] = zaxis.v.y, viewMatrix[2][2] = zaxis.v.z;
	viewMatrix[3][0] = position.x, viewMatrix[3][1] = position.y, viewMatrix[3][2] = position.z;
}