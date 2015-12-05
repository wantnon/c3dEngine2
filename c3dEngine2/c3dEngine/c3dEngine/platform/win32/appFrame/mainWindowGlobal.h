#ifndef _mainWindowGlobal_H
#define _mainWindowGlobal_H
#include <windows.h>
//global var
extern bool isTouching;
extern long tf,t,dt;
extern float captionHeight;
extern float frameBoarder;

//win32 global var
extern HDC			hDC;
extern HGLRC		hRC;
extern HWND		hWnd;
extern HINSTANCE	hInstance;
#endif