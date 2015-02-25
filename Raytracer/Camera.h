#ifndef _CAMERA_H
#define _CAMERA_H

#include "World.h"

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
	Camera(void);
	~Camera(void);
	void render(World world, HDC hdc);
};
#endif