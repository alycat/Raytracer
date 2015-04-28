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
	PlyFile *ply;
	int nelems;
	char **elist;
	int file_type;
	float version;
	int nprops;
	int num_elems;
	PlyProperty **plist;
	Vertex **vlist = NULL;
	Face **flist = NULL;
	char *elem_name;
	int num_comments;
	char **comments;
	int num_obj_info;
	char **obj_info;
	
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
				Point offset = { 0, 0, 5 };
				bunny.push_back(new Triangle(p1*10 + offset, p2*10 + offset, p3*10 + offset));
			}
		}
	}

	ply_close(ply);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	cam = new Camera();
	wrld = new World();
	Color p = { 1, 0, 1 };
	Sphere *s1 = new Sphere({ 0.0, 0.2, 8 }, 0.85, white);
	Sphere *s2 = new Sphere({ -1.3, 0.7, 6 }, 1, white);
	Triangle * t1 = new Triangle({ 4.0, -3, 0 }, { -4.0, 1.5, 22 }, { 4.0, 1.5, 22 }, {-4.0, 4.0, 0, 22});
	Triangle * t2 = new Triangle({ 4.0, -3, 0 },{ -4.0, -3, 0 },{ -4.0, 1.5, 22 },   { -4.0, 4.0, 0, 22 });
	Sphere *s3 = new Sphere({0, -5, 7}, 5, grey);
	s1->k_r = 0.3;
	s2->k_t = 0.2;
	/*
	ReadPlyFile("bunny");
	for (int i = 0; i < 100; ++i){
		wrld->add(bunny[i]);
	}*/

	/*
	vector<Sphere*> spheres;
	for (int j = 0; j < 3; ++j){
		for (int i = 0; i < 5; ++i){
			spheres.push_back(new Sphere({ -i, j, 10 }, 0.4, green));
			wrld->add(spheres[i + (j*5)]);
		}
	}
	vector<Sphere*> spheres2;
	for (int j = 0; j < 10; ++j){
		for (int i = 0; i < 5; ++i){
			spheres2.push_back(new Sphere({ i, j, 2 }, 0.4, green));
			wrld->add(spheres2[i + (j * 5)]);
		}
	}*/
	
	wrld->add(s1);
	wrld->add(s2);
	wrld->add(t1);
	wrld->add(t2);
	//wrld->add(s3);

	LightSource* l1 = new LightSource({ { -1, 12.0f, -5 }, white, { grey } }); //position, color, light
	wrld->add(l1);
	LightSource* l2 = new LightSource({ { -1.7, 1, 5 }, white/100, { black } });
	wrld->add(l2);
	LightSource* l3 = new LightSource({ { 0, 2.6, 6 }, white/100, { white } });
	wrld->add(l3);
	LightSource* l4 = new LightSource({ { 0, -5, -2 }, white/100, { white } });
	wrld->add(l4);

	wrld->initTree();

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
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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