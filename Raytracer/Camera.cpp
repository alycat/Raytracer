#include "Camera.h"

Camera::Camera(){
	filmplane = { 500, 500, 1000, 1000, 15 };
	lookat = { {0.0, 0.0, 1.0} };
	position = {-0.15, 0, 0};
	up = { { 0.0, 1.0, 0.0 } };
	pVector zaxis = lookat - position;
	zaxis.normalize();
	pVector xaxis = up;
	xaxis.cross(zaxis);
	pVector yaxis = xaxis;
	yaxis.cross(zaxis * -1);
	viewMatrix = Matrix(4, 4);
	viewMatrix[0][0] = xaxis.v.x, viewMatrix[0][1] = xaxis.v.y, viewMatrix[0][2] = xaxis.v.z;
	viewMatrix[1][0] = yaxis.v.x, viewMatrix[1][1] = yaxis.v.y, viewMatrix[1][2] = yaxis.v.z;
	viewMatrix[2][0] = zaxis.v.x, viewMatrix[2][1] = zaxis.v.y, viewMatrix[2][2] = zaxis.v.z;
	viewMatrix[3][0] = position.x, viewMatrix[3][1] = position.y, viewMatrix[3][2] = position.z;
}

Camera::~Camera(){

}

void Camera::render(World world, HDC hdc){
	//world.transformAllObjects(viewMatrix);
	float pixelW = filmplane.w/filmplane.W;
	float pixelH = filmplane.h / filmplane.H;
	Point top_left = {-filmplane.w/2 + pixelW/2, filmplane.h/2 - pixelH/2, filmplane.f};
	float sx = top_left.x;
	float px = sx;
	float py = top_left.y;

	for (int y = 0; y < filmplane.H; ++y){
		px = sx;
		for (int x = 0; x < filmplane.W; ++x){
			Ray ray = { position, { px, py, filmplane.f } };
			ray.direction.normalize();
			COLORREF color = world.trace(ray);
			SetPixel(hdc, x, y, color);
			px += pixelW;
		}
		py -= pixelH;
	}
}