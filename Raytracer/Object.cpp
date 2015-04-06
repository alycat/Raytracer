#include "Object.h"

Object::Object(void){
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
	material = new Phong();
}

Object::~Object(void){
	if (material){
		delete material;
	}
	material = nullptr;
}

Object::Object(Color c){
	color = c;
	material = new Phong();
}

Point Object::intersect(Ray ray){
	cout << "Object\n";
	return maxPoint;
}

void Object::transform(Matrix matrix){

}

pVector Object::normal(Point p)
{
	return{ origin };
}