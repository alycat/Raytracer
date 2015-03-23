#include "Camera.h"

Camera::Camera(){
	filmplane = { 500, 500, 150000, 150000, 5 };
	//lookat = { {0.0, -1, 1.0} };
	//position = {1, 1, -1.5};
	position = { 0, 0, 0 };
	lookat = { { 0, 0.0, 1.0 } };
	up = { { 0.0, 1.0, 0.0 } };

	pVector n = lookat - position;
	n.scalar(-1);
	n.normalize();
	pVector u = up.getCross(n);
	u.normalize();
	pVector v = n.getCross(u);
	v.normalize();
	viewMatrix = Matrix(4, 4);
	pVector eyeP = { position };
	eyeP.scalar(-1);
	eyeP.normalize();

	viewMatrix[0][0] = u.v.x, viewMatrix[0][1] = v.v.x, viewMatrix[0][2] = n.v.x, viewMatrix[0][3] = 0;
	viewMatrix[1][0] = u.v.y, viewMatrix[1][1] = v.v.y, viewMatrix[1][2] = n.v.y, viewMatrix[1][3] = 0;
	viewMatrix[2][0] = u.v.z, viewMatrix[2][1] = v.v.z, viewMatrix[2][2] = n.v.z, viewMatrix[2][3] = 0;
	viewMatrix[3][0] = u.dot(eyeP), viewMatrix[3][1] = v.dot(eyeP), viewMatrix[3][2] = n.dot(eyeP), viewMatrix[3][3] = 1;/*
	
	viewMatrix[0][0] = u.v.x, viewMatrix[0][1] = u.v.y, viewMatrix[0][2] = u.v.z, viewMatrix[0][3] = u.dot(eyeP);
	viewMatrix[1][0] = v.v.x, viewMatrix[1][1] = v.v.y, viewMatrix[1][2] = v.v.z, viewMatrix[1][3] = v.dot(eyeP);
	viewMatrix[2][0] = n.v.x, viewMatrix[2][1] = n.v.y, viewMatrix[2][2] = n.v.z, viewMatrix[2][3] = n.dot(eyeP);
	viewMatrix[3][0] = 0, viewMatrix[3][1] = 0, viewMatrix[3][2] = 0, viewMatrix[3][3] = 1;*/
	
}

Camera::~Camera(){

}

void Camera::render(World world, HDC hdc){
	world.transformAllObjects(viewMatrix);
	float w = filmplane.w;
	float h = filmplane.h;
	int H = filmplane.H;
	int W = filmplane.W;
	float pixelW = w/W;
	float pixelH = h/H;
	Point top_left = {(-filmplane.w + pixelW)/2, (filmplane.h - pixelH)/2, filmplane.f};
	//top_left = {(filmplane.h - pixelW)/2, (-filmplane.h + pixelH)/2, filmplane.f};
	float sx = top_left.x;
	float px = sx;
	float py = top_left.y;

	for (int y = 0; y < filmplane.H; ++y){
		px = sx;
		for (int x = 0; x < filmplane.W; ++x){
			Ray ray = { origin, { px, py, filmplane.f } };
			ray.direction.normalize();
			COLORREF color = world.trace(ray);
			SetPixel(hdc, x, y, color);
			px += pixelW;
		}
		py -= pixelH;
	}
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