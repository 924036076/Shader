#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "texture.h"
#include "utils.h"
#include "objmodel.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	//register window
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszClassName = "GLWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom)
	{
		MessageBox(NULL, "Register Fail", "Error", MB_OK);
		return 0;
	}

	//create window
	HWND hwnd = CreateWindowEx(NULL, "GLWindow", "OpneGL Window", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);

	//create OpenGL render context
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int pixexFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixexFormat, &pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc); // setup OpenGL context complete

	//OpenGL init
	glMatrixMode(GL_PROJECTION); //tell the GPU processor that I would select the projection matrix GPU处理器
	gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);//set some values to projection matrix
	glMatrixMode(GL_MODELVIEW); //tell ... model view matrix
	glLoadIdentity();

	Texture texture;
	texture.Init("res/Greeting.bmp");//init OpenGL texture
	ObjModel model;
	model.Init("res/Cube.obj");

	glClearColor(0.1f, 0.4f, 0.6f, 1.0f); //set clear color for background
	//show window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	glEnable(GL_CULL_FACE);

	//init light
	float blackColor[] = { 0.0f,0.0f,0.0f,1.0f };
	float whiteColor[] = { 1.0f,1.0f,1.0f,1.0f };
	float lightPos[] = { 0.0f,1.0f,0.0f,0.0f };//direction light
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	float blackMat[] = { 0.0f,0.0f,0.0f,1.0f };
	float ambientMat[] = { 0.1f,0.1f,0.1f,1.0f };
	float diffuseMat[] = { 0.4f,0.4f,0.4f,1.0f };
	float specularMat[] = { 0.9f,0.9f,0.9f,1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//front face: ccw -> counter clock wind
	MSG msg;
	while (true)
	{
		//抓取所有消息
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			//如果接收到退出消息
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//draw scene
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);
		model.Draw();
		//present scene
		SwapBuffers(dc);	//交换缓冲区
	}
	return 0;
}