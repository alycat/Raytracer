#ifndef _CAMERA_H
#define _CAMERA_H

#include "World.h"
#include <thread>

class Camera{
private:
	struct FilmPlane{
		int W;
		int H;
		float w;
		float h;
		float f;
	};
public:
	Matrix viewMatrix;
	FilmPlane filmplane;
	Point position;
	pVector lookat;
	pVector up;
	pVector u, v, n;
	pVector middleScreen;
	Camera(void);
	~Camera(void);
	void render(World world, HDC hdc);
	void update(Point np);
};
#endif