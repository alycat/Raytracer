// Raytracer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Raytracer.h"
#include "Camera.h"
#include "ply.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
Camera* cam;
World* wrld;
Point position;
vector<Triangle*> bunny;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID				ReadPlyFile(char* filepath);
VOID				SetupCornellBox();
VOID				SetupWhittedScene();
VOID				SetupStanfordRabbit();
VOID				SetupScatter();

char *elem_names[] = { /* list of the kinds of elements in the user's object */
	"vertex", "face"
};

typedef struct Vertex{
	float x;
	float y;
	float z;
	float confidence;
	float intensity;
};

typedef struct Face {
	unsigned char nverts;    /* number of vertex indices in list */
	int *verts;              /* vertex index list */
};


PlyProperty vert_props[] = { /* list of property information for a vertex */
		{ "x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, x), 0, 0, 0, 0 },
		{ "y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, y), 0, 0, 0, 0 },
		{ "z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, z), 0, 0, 0, 0 },
		{ "confidence", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, confidence), 0, 0, 0, 0 },
		{ "intensity", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, intensity), 0, 0, 0, 0 },
};

PlyProperty face_props[] = { /* list of property information for a vertex */
		{ "vertex_indices", PLY_INT, PLY_INT, offsetof(Face, verts),
		1, PLY_UCHAR, PLY_UCHAR, offsetof(Face, nverts) },
};




void ReadPlyFile(char* filepath){
	int i, j, k;
	PlyFile *ply = NULL;
	int nelems;
	char **elist = NULL;
	int file_type;
	float version;
	int nprops;
	int num_elems;
	PlyProperty **plist = NULL;
	Vertex **vlist = NULL;
	Face **flist = NULL;
	char *elem_name = NULL;
	int num_obj_info;
	
	ply = ply_open_for_reading(filepath, &nelems, &elist, &file_type, &version);

	for (int i = 0; i < nelems; ++i){
		/* get the description of the first element */
		elem_name = elist[i];
		plist = ply_get_element_description(ply, elem_name, &num_elems, &nprops);
		/* if we're on vertex elements, read them in */
		if (equal_strings("vertex", elem_name)) {

//			vlist = (Point **)malloc(sizeof(Point *) * num_elems);
			/* create a vertex list to hold all the vertices */
			vlist = (Vertex **)malloc(sizeof(Vertex *) * num_elems);
			/* set up for getting vertex elements */
			ply_get_property(ply, elem_name, &vert_props[0]);
			ply_get_property(ply, elem_name, &vert_props[1]);
			ply_get_property(ply, elem_name, &vert_props[2]);
			/* grab all the vertex elements */
			for (j = 0; j < num_elems; j++) {
				vlist[j] = (Vertex *)malloc(sizeof(Vertex));
				ply_get_element(ply, (void *)vlist[j]);
			}
		}
		else if (equal_strings("face", elem_name)){
			flist = (Face**)malloc(sizeof(Face*)*num_elems);
			ply_get_property(ply, elem_name, &face_props[0]);

			for (k = 0; k < num_elems; ++k){
				flist[k] = (Face*)malloc(sizeof(Face));
				ply_get_element(ply, (void*)flist[k]);
				cout << flist[k] << endl;
				Point p1 = { vlist[flist[k]->verts[0]]->x, vlist[flist[k]->verts[0]]->y, vlist[flist[k]->verts[0]]->z };
				Point p2 = { vlist[flist[k]->verts[1]]->x, vlist[flist[k]->verts[1]]->y, vlist[flist[k]->verts[1]]->z };
				Point p3 = { vlist[flist[k]->verts[2]]->x, vlist[flist[k]->verts[2]]->y, vlist[flist[k]->verts[2]]->z };
				Point offset = { -1, -0.8, 4 };
				bunny.push_back(new Triangle(p1*10 + offset, p2*10 + offset, p3*10 + offset));
				//wrld->add(new Triangle(p1 * 10 + offset, p2 * 10 + offset, p3 * 10 + offset));
			}
		}
	}
	if (ply){
		ply_close(ply);
		//delete ply;
	}
	if (plist){
		delete[] plist;
	}
	if (elist){
		delete[] elist;
	}
	if (vlist){
		delete[] vlist;
	}
	if (flist){
		delete[] flist;
	}
	if (elem_name){
		delete elem_name;
	}
}

void SetupCornellBox(){
	wrld->setBackgroundColor(black);
	cam->position = origin;

	Sphere *s3 = new Sphere({ -2, 0.5, 12 }, 1.8, white);
	Sphere *s4 = new Sphere({ 2, 1.5, 19.5 }, 1.8, white);

	s3->k_t = 0.75;
	s4->k_t = 0;
	s4->k_r = 0.75;

	s3->material->kd = 0.0001;
	s3->material->ks = 0.75;
	s3->material->ke = 0.9;

	s4->material->kd = 0.0001;
	s4->material->ks = 0.75;
	s4->material->ke = 1;

	Triangle * t3 = new Triangle({ 4.0, -3, 0 }, { -4.0, 0, 22 }, { 4.0, 0, 22 });
	Triangle * t4 = new Triangle({ 4.0, -3, 0 }, { -4.0, -3, 0 }, { -4.0, 0, 22 });
	t3->material->c = white;
	t4->material->c = white;

	Triangle *t5 = new Triangle({ 4.0, 0, 22 }, { -4.0, 0, 22 }, { 4, 5, 22 });
	Triangle *t6 = new Triangle({ 4.0, 5, 22 }, { -4, 0, 22 }, { -4.0, 5, 22 });

	Triangle *t7 = new Triangle({ 4.0, 2, 0 }, { 4.0, -3, 0 }, { 4, 5, 22 });
	Triangle *t8 = new Triangle({ 4.0, -3, 0 }, { 4, 0, 22 }, { 4.0, 5, 22 });
	t7->material->c = blue;
	t8->material->c = blue;

	Triangle *t9 = new Triangle({ -4.0, -3, 0 }, { -4.0, 2, 0 }, { -4, 5, 22 });
	Triangle *t10 = new Triangle({ -4.0, -3, 0 }, { -4.0, 5, 22 }, { -4, 0, 22 });
	t9->material->c = red;
	t10->material->c = red;

	Triangle * t11 = new Triangle({ 4.0, 3.36, 10 }, { -4.0, 3.36, 10 }, { 4.0, 2, 0 });
	Triangle * t12 = new Triangle({ -4.0, 3.36, 10 }, { -4.0, 2, 0 }, { 4.0, 2, 0 });
	t11->material->c = white;
	t12->material->c = white;

	Triangle * t13 = new Triangle({ 4.0, 5, 22 }, { 4.0, 4.32, 17 }, { -4.0, 5, 22 });
	Triangle * t14 = new Triangle({ -4.0, 5, 22 }, { -4.0, 4.32, 17 }, { 4.0, 4.32, 17 });
	t13->material->c = white;
	t14->material->c = white;

	Triangle * t15 = new Triangle({ 1.0, 5, 17 }, { 1.0, 4.32, 17 }, { -1.0, 5, 17 });
	Triangle * t16 = new Triangle({ -1.0, 5, 17 }, { 1.0, 4.32, 17 }, { -1.0, 4.32, 17 });
	t15->material->c = white;
	t16->material->c = white;
	t15->k_r = 0.5;
	t16->k_r = 0.5;

	Triangle * t17 = new Triangle({ 1.0, 5, 17 }, { 1.0, 3.36, 10 }, { 1.0, 4.32, 17 });
	Triangle * t18 = new Triangle({ 1.0, 3.36, 10 }, { 1.0, 4.04, 10 }, { 1.0, 5, 17 });
	t17->material->c = white;
	t18->material->c = white;
	t17->k_r = 0.5;
	t18->k_r = 0.5;

	Triangle * t19 = new Triangle({ -1.0, 5, 17 }, { -1.0, 4.32, 17 }, { -1.0, 3.36, 10 });
	Triangle * t20 = new Triangle({ -1.0, 3.36, 10 }, { -1.0, 5, 17 }, { -1.0, 4.04, 10 });
	t19->material->c = white;
	t20->material->c = white;
	t19->k_r = 0.5;
	t20->k_r = 0.5;

	Triangle * t21 = new Triangle({ -1.0, 5, 17 }, { -1.0, 4.04, 10 }, { 1.0, 5, 17 });
	Triangle * t22 = new Triangle({ 1.0, 4.04, 10 }, { -1.0, 4.04, 10 }, { 1.0, 5, 17 });
	t21->material->c = white;
	t22->material->c = white;

	Triangle * t23 = new Triangle({ -4.0, 4.32, 17 }, { -4.0, 3.36, 10 }, { -1.0, 4.32, 17 });
	Triangle * t24 = new Triangle({ -4.0, 3.36, 10 }, { -1.0, 3.36, 10 }, { -1.0, 4.32, 17 });
	t23->material->c = white;
	t24->material->c = white;


	Triangle * t25 = new Triangle({ 4.0, 4.32, 17 }, { 1.0, 4.32, 17 }, { 4.0, 3.36, 10 });
	Triangle * t26 = new Triangle({ 4.0, 3.36, 10 }, { 1.0, 4.32, 17 }, { 1.0, 3.36, 10 });
	t25->material->c = white;
	t26->material->c = white;

	Triangle * t27 = new Triangle({ -1.0, 4.32, 17 }, { -1.0, 3.36, 10 }, { 1.0, 4.32, 17 });
	Triangle * t28 = new Triangle({ 1.0, 3.36, 10 }, { 1.0, 4.32, 17 }, { -1.0, 3.36, 10 });
	t27->material->c = white;
	t28->material->c = white;
	t27->k_t = 0.5;
	t28->k_t = 0.5;

	wrld->add(s3);
	wrld->add(s4);
	wrld->add(t3);
	wrld->add(t4);
	wrld->add(t5);
	wrld->add(t6);
	wrld->add(t7);
	wrld->add(t8);
	wrld->add(t9);
	wrld->add(t10);
	wrld->add(t11);
	wrld->add(t12);
	wrld->add(t13);
	wrld->add(t14);
	wrld->add(t15);
	wrld->add(t16);
	wrld->add(t17);
	wrld->add(t18);
	wrld->add(t19);
	wrld->add(t20);

	//wrld->add(t21);
	//wrld->add(t22);

	wrld->add(t23);
	wrld->add(t24);
	wrld->add(t25);
	wrld->add(t26);

	//wrld->add(t27);
	//wrld->add(t28);

	/*
	LightSource* l5 = new LightSource({ { 0.25, 4, 15 }, white, { white * 1000 }, 100000 });
	wrld->add(l5);*/
	LightSource* l6 = new LightSource({ {0, 4, 15.75 }, white, { white * 1000 }, 10000 });
	wrld->add(l6);

	wrld->initTree();
	wrld->emitPhotons();
	wrld->emitCaustic();
}

void SetupWhittedScene(){
	cam->position = { -1.2, 0.0, 0 };
	Sphere *s1 = new Sphere({ 0.0, 0.2, 8 }, 0.85, white);
	Sphere *s2 = new Sphere({ -1.3, 0.7, 6 }, 1, white);
	Triangle * t1 = new Triangle({ 4.0, -3, 0 }, { -4.0, 1.5, 22 }, { 4.0, 1.5, 22 }, { -4.0, 4.0, 0, 22 });
	Triangle * t2 = new Triangle({ 4.0, -3, 0 }, { -4.0, -3, 0 }, { -4.0, 1.5, 22 }, { -4.0, 4.0, 0, 22 });
	s1->k_r = 0.5;
	s2->k_t = 0.5;
	s1->material->kd = 0.0001;
	s1->material->ks = 0.75;
	s1->material->ke = 0.9;

	s2->material->kd = 0.0001;
	s2->material->ks = 0.7;
	s2->material->ke = 1;

	t1->material->ks = 0.0;
	t1->material->kd = 0.5;

	t2->material->ks = 0.0;
	t2->material->kd = 0.5;

	wrld->add(s1);
	wrld->add(s2);
	wrld->add(t1);
	wrld->add(t2);

	LightSource* l1 = new LightSource({ { -1, 12.0f, -5 }, white, { white }, 500000 }); //position, color, light
	wrld->add(l1);
	LightSource* l2 = new LightSource({ { -1.7, 1, 5 }, white / 100, { black }, 1000 });
	wrld->add(l2);
	LightSource* l3 = new LightSource({ { 0, 2.6, 6 }, white / 100, { grey }, 1000 });
	wrld->add(l3);
	LightSource* l4 = new LightSource({ { 0, -5, -2 }, white / 100, { grey }, 1000 });
	wrld->add(l4);

	wrld->initTree();
	wrld->emitPhotons();
}

void SetupStanfordRabbit(){
	cam->position = { -1.2, 0.0, 0 };
	ReadPlyFile("bunny");
	for (int i = 0; i < bunny.size(); ++i){
		bunny[i]->material->ks = 0.75;
		bunny[i]->material->ke = 0.9;
		bunny[i]->material->kd = 0.0001;
		wrld->add((bunny[i]));
	}
	LightSource* l1 = new LightSource({ { -1, 12.0f, -5 }, white, { white }, 0 }); //position, color, light
	wrld->add(l1);
	LightSource* l2 = new LightSource({ { -1.7, 1, 5 }, white / 100, { black }, 0 });
	wrld->add(l2);
	LightSource* l3 = new LightSource({ { 0, 2.6, 6 }, white / 100, { grey }, 0 });
	wrld->add(l3);
	LightSource* l4 = new LightSource({ { 0, -5, -2 }, white / 100, { grey }, 0 });
	wrld->add(l4);
	
	wrld->initTree();
}

void SetupScatter(){
	wrld->setBackgroundColor(black);
	cam->position = origin;

	Sphere *s3 = new Sphere({ -2, 0.5, 12 }, 1.8, white);
	Sphere *s4 = new Sphere({ 0, 1.5, 19.5 }, 1.8, white);
	s3->material = new Rayleigh();
	s4->material = new Rayleigh();
	s3->k_t = 0;
	s3->k_r = 0;
	s4->k_t = 0.3;
	s4->k_r = 0.3;
	//s4->color = purple;
	//s3->k_t = 0.75;
	//s4->k_t = 0;
	//s4->k_r = 0.75;

	s3->material->kd = 0.0001;
	s3->material->ks = 0.75;
	s3->material->ke = 0.9;

	s4->material->kd = 0.001;
	s4->material->ks = 0.1;
	s4->material->ke = 1;

	Triangle * t3 = new Triangle({ 4.0, -3, 0 }, { -4.0, 0, 22 }, { 4.0, 0, 22 });
	Triangle * t4 = new Triangle({ 4.0, -3, 0 }, { -4.0, -3, 0 }, { -4.0, 0, 22 });
	t3->material->c = white;
	t4->material->c = white;

	Triangle *t5 = new Triangle({ 4.0, 0, 22 }, { -4.0, 0, 22 }, { 4, 5, 22 });
	Triangle *t6 = new Triangle({ 4.0, 5, 22 }, { -4, 0, 22 }, { -4.0, 5, 22 });
	t5->material->c = white;
	t6->material->c = white;

	Triangle *t7 = new Triangle({ 4.0, 2, 0 }, { 4.0, -3, 0 }, { 4, 5, 22 });
	Triangle *t8 = new Triangle({ 4.0, -3, 0 }, { 4, 0, 22 }, { 4.0, 5, 22 });
	t7->material->c = white;
	t8->material->c = white;

	Triangle *t9 = new Triangle({ -4.0, -3, 0 }, { -4.0, 2, 0 }, { -4, 5, 22 });
	Triangle *t10 = new Triangle({ -4.0, -3, 0 }, { -4.0, 5, 22 }, { -4, 0, 22 });
	t9->material->c = white;
	t10->material->c = white;

	Triangle * t11 = new Triangle({ 4.0, 3.36, 10 }, { -4.0, 3.36, 10 }, { 4.0, 2, 0 });
	Triangle * t12 = new Triangle({ -4.0, 3.36, 10 }, { -4.0, 2, 0 }, { 4.0, 2, 0 });
	t11->material->c = white;
	t12->material->c = white;

	Triangle * t13 = new Triangle({ 4.0, 5, 22 }, { 4.0, 4.32, 17 }, { -4.0, 5, 22 });
	Triangle * t14 = new Triangle({ -4.0, 5, 22 }, { -4.0, 4.32, 17 }, { 4.0, 4.32, 17 });
	t13->material->c = white;
	t14->material->c = white;

	Triangle * t15 = new Triangle({ 1.0, 5, 17 }, { 1.0, 4.32, 17 }, { -1.0, 5, 17 });
	Triangle * t16 = new Triangle({ -1.0, 5, 17 }, { 1.0, 4.32, 17 }, { -1.0, 4.32, 17 });
	t15->material->c = white;
	t16->material->c = white;
	t15->k_r = 0.5;
	t16->k_r = 0.5;

	Triangle * t17 = new Triangle({ 1.0, 5, 17 }, { 1.0, 3.36, 10 }, { 1.0, 4.32, 17 });
	Triangle * t18 = new Triangle({ 1.0, 3.36, 10 }, { 1.0, 4.04, 10 }, { 1.0, 5, 17 });
	t17->material->c = white;
	t18->material->c = white;
	t17->k_r = 0.5;
	t18->k_r = 0.5;

	Triangle * t19 = new Triangle({ -1.0, 5, 17 }, { -1.0, 4.32, 17 }, { -1.0, 3.36, 10 });
	Triangle * t20 = new Triangle({ -1.0, 3.36, 10 }, { -1.0, 5, 17 }, { -1.0, 4.04, 10 });
	t19->material->c = white;
	t20->material->c = white;
	t19->k_r = 0.5;
	t20->k_r = 0.5;

	Triangle * t21 = new Triangle({ -1.0, 5, 17 }, { -1.0, 4.04, 10 }, { 1.0, 5, 17 });
	Triangle * t22 = new Triangle({ 1.0, 4.04, 10 }, { -1.0, 4.04, 10 }, { 1.0, 5, 17 });
	t21->material->c = white;
	t22->material->c = white;

	Triangle * t23 = new Triangle({ -4.0, 4.32, 17 }, { -4.0, 3.36, 10 }, { -1.0, 4.32, 17 });
	Triangle * t24 = new Triangle({ -4.0, 3.36, 10 }, { -1.0, 3.36, 10 }, { -1.0, 4.32, 17 });
	t23->material->c = white;
	t24->material->c = white;


	Triangle * t25 = new Triangle({ 4.0, 4.32, 17 }, { 1.0, 4.32, 17 }, { 4.0, 3.36, 10 });
	Triangle * t26 = new Triangle({ 4.0, 3.36, 10 }, { 1.0, 4.32, 17 }, { 1.0, 3.36, 10 });
	t25->material->c = white;
	t26->material->c = white;

	Triangle * t27 = new Triangle({ -1.0, 4.32, 17 }, { -1.0, 3.36, 10 }, { 1.0, 4.32, 17 });
	Triangle * t28 = new Triangle({ 1.0, 3.36, 10 }, { 1.0, 4.32, 17 }, { -1.0, 3.36, 10 });
	t27->material->c = white;
	t28->material->c = white;
	t27->k_t = 0.5;
	t28->k_t = 0.5;

	//wrld->add(s3);
	wrld->add(s4);
	wrld->add(t3);
	wrld->add(t4);
	wrld->add(t5);
	wrld->add(t6);
	wrld->add(t7);
	wrld->add(t8);
	wrld->add(t9);
	wrld->add(t10);
	wrld->add(t11);
	wrld->add(t12);
	wrld->add(t13);
	wrld->add(t14);
	wrld->add(t15);
	wrld->add(t16);
	wrld->add(t17);
	wrld->add(t18);
	wrld->add(t19);
	wrld->add(t20);

	//wrld->add(t21);
	//wrld->add(t22);

	wrld->add(t23);
	wrld->add(t24);
	wrld->add(t25);
	wrld->add(t26);

	//wrld->add(t27);
	//wrld->add(t28);

	/*
	LightSource* l5 = new LightSource({ { 0.25, 4, 15 }, white, { white * 1000 }, 100000 });
	wrld->add(l5);*/
	LightSource* l6 = new LightSource({ { 0, 2, 16 }, white, { white * 1000 }, 100000 });
	wrld->add(l6);

	wrld->initTree();
	wrld->emitScatter();
	//wrld->emitCaustic();
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	cam = new Camera();
	wrld = new World();
	Color p = { 1, 0, 1 };


	//SetupCornellBox();
	SetupCornellBox();
	/*
	Sphere *rayleigh = new Sphere({ 0.0, 0.5, 10 }, 2, 0);
	rayleigh->k_r = 0.5;
	rayleigh->k_t = 0.5;
	wrld->add(rayleigh);
	Sphere *rayleigh2 = new Sphere({ 1, 0.5, 12 }, 1, 0);
	rayleigh2->k_r = 0.5;
	rayleigh2->k_t = 0.5;
	wrld->add(rayleigh2);
	Sphere *rayleigh3 = new Sphere({ -1, 0.5, 14 }, 1, 0);
	rayleigh3->k_r = 0.5;
	rayleigh3->k_t = 0.5;
	wrld->add(rayleigh3);
	Sphere *rayleigh4 = new Sphere({ 0, 0.5, 16 }, 1, 0);
	rayleigh4->k_r = 0.5;
	rayleigh4->k_t = 0.5;
	wrld->add(rayleigh4);
	*/

/*
	LightSource* light = new LightSource({ {1, 1.0f, 0 }, white*1000, { white }, 1000 }); //position, color, light
	wrld->add(light);*/
/*	LightSource* light2 = new LightSource({ { 0.5, 3.0f, 5 }, orange * 100, { white }, 0 }); //position, color, light
	wrld->add(light2);
	Color test = { 255, 100, 100 };*/
	/*LightSource* light3 = new LightSource({ { 0, 0.0f, 0 },  white*100, { white }, 0 }); //position, color, light
	wrld->add(light3*/
	
/*

*/


	/*LightSource* l6 = new LightSource({ { 0, 2, 15 }, white/2, { white }, 4000 });
	wrld->add(l6);*/
	/*
	LightSource* l7 = new LightSource({ { 0, 0, 0 }, white, { white }, 1000 });
	wrld->add(l7);
	*/
	
	//wrld->emitScatter();
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAYTRACER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		/*if ((GetKeyState(VK_LEFT) & 0x8000) != 0){
			position.x--;
			cam->update(position);
			InitInstance(hInstance, nCmdShow);
		}*/
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAYTRACER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT recWnd = { 0, 0, cam->filmplane.W, cam->filmplane.H };

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 200, recWnd.right - recWnd.left, recWnd.bottom - recWnd.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

  

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if ((GetKeyState(VK_LEFT) & 0x8000) != 0)
	{
		position.x--;
	}
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			_CrtDumpMemoryLeaks();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		cam->render(*wrld, hdc);
		delete cam;
		delete wrld;
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		_CrtDumpMemoryLeaks();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}