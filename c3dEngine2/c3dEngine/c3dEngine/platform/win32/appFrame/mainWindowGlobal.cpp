#include "mainWindowGlobal.h"
//global var
bool isTouching;
long tf=0,t=0,dt=0;//tf上次进入idle,t本次进入idle,dt本次与上次进入idle时间间隔--abc
float captionHeight=0;//标题栏高度--abc
float frameBoarder=0;//窗口边框宽度--abc
 
//win32 global var
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application