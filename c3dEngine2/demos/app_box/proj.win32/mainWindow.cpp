#include "resource.h "

//
#include <string>
#include <vector>
#include <iostream> 
#include <fstream>
using namespace std;
#include <Tchar.h>//_T()函数在此头文件--abc
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <float.h>//_isnan
#include <string.h>
//
#include "c3d.h"
//
#include "initGame.h"


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,_T("Release Of DC And RC Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,_T("Release Rendering Context Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,_T("Release Device Context Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,_T("Could Not Release hWnd."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(_T("OpenGL"),hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,_T("Could Not Unregister Class."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*/
BOOL CreateGLWindow(LPCWSTR title, int winwidth, int winheight, int nbit_colorDepth,int nbit_zDepth)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)winwidth;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)winheight;		// Set Bottom Value To Requested Height


	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MENU2);//菜单--abc
	wc.lpszClassName	= _T("OpenGL");								// Set The Class Name



	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,_T("Failed To Register The Window Class."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								_T("OpenGL"),							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Window Creation Error."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		nbit_colorDepth,							// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,//alpha缓存位数,目前未使用alpha测试,所以置为0。若将来使用,不要忘记修改此项!!
		0,											// Shift Bit Ignored
		0,											// Accumulation Buffer位数--abc
		0, 0, 0, 0,									// Accumulation Bits Ignored
		nbit_zDepth,								// Z-Buffer (Depth Buffer)  
		8,//模板缓存位数--abc
		0,											// Auxiliary Buffer位数--abc
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Create A GL Device Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Find A Suitable PixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Set The PixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Create A GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Activate The GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	return TRUE;									// Success
}



LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;						
		}
		case WM_COMMAND:
		{
			return 0;
			
		}

		case WM_KEYDOWN:						
		{
			keyDown( hWnd, uMsg, wParam, lParam);
			return 0;							
		}
			
		case WM_KEYUP:								
		{
			keyUp( hWnd, uMsg, wParam, lParam);
			return 0;								
		}
		case WM_LBUTTONDOWN:
		{
			lbuttonDown( hWnd, uMsg, wParam, lParam);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			lbuttonUp( hWnd, uMsg, wParam, lParam);

			return 0;
		}
		case WM_MOUSEMOVE:
		{
			mouseMove( hWnd, uMsg, wParam, lParam);

			return 0;
		}

		case  WM_MOUSELEAVE://not WM_NCMOUSELEAVE
		{
			mouseLeave( hWnd, uMsg, wParam, lParam);
			return 0;
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			winSize( hWnd, uMsg, wParam, lParam);
			
			
			return 0;								
		}
		case WM_CREATE: 
		{
	
			return 0;
		}
		
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	//创建并初始化控制台--abc
	createAndInitConsole();
	


	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	//标题栏高度--abc
	//http://baike.baidu.com/link?url=jsZlb4o0MUkNjSDUiKiEg7yqz-PyaVPQkh8SsKCrrCmctxxNlGCy1zDe61VofSE0iN5P_aw8Dv6KD7fDi0eXh_
	captionHeight=GetSystemMetrics(SM_CYCAPTION);
	cout<<"captionHeight:"<<captionHeight<<endl;
	//边框高度--abc
	frameBoarder=GetSystemMetrics(SM_CYSIZEFRAME);
	cout<<"frameBoarder:"<<frameBoarder<<endl;
		
	// Create Our OpenGL Window
	int nbit_colorDepth=24;
	int nbit_zDepth=32;
	float width = Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x()+2*frameBoarder;
	float height = Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()+2*frameBoarder+captionHeight;
	cout<<"window size:"<<width<<" "<<height<<endl;
	if (!CreateGLWindow((LPCWSTR)_T("c3d"),width,height,nbit_colorDepth,nbit_zDepth))
	{
		return 0;									// Quit If Window Was Not Created
	}
	

	//init game frame
	initWithFrame();
	//init game
	initGame();
	//init time
	tf=t=clock();
	

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{//--------------------------idle
			tf=t;
			t=clock();
			dt=t-tf;//上一次进入idle到本次进入idle的时间间隔--abc
			//逻辑更新中所有的位移量都要以dt为参考进行,以保证物体移动速度与帧率无关--abc
			//但目前还没有实现用dt为参考--abc
			render(hDC);
		}
	}

	// Shutdown
	cout<<"quit program..."<<endl;
	teardownOpenAL();
	FreeConsole();									// 销毁控制台--abc
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
