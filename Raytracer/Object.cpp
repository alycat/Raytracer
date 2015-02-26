#include "Object.h"


Object::Object(void){
	color = black;
	vM = Matrix(4, 4);
	vM.unit();
}

Object::~Object(void){

}

Object::Object(Color c){
	color = c;
}

Point Object::intersect(Ray ray){
	cout << "Object\n";
	return maxPoint;
}

void Object::transform(Matrix matrix){

}