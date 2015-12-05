#ifndef _mainWindowFuncs_H
#define _mainWindowFuncs_H
#include <windows.h>
#include <time.h>
#include <Tchar.h>//_T()函数在此头文件\

#include "common/c3dAL.h"
#include "common/c3dGL.h"
//
#include "core/c3dInitGame.h"
//
//#include "resource.h "
//
#include "platform/win32/appFrame/mainWindowGlobal.h"
//
void keyDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void keyUp(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void lbuttonDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void lbuttonUp(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void mouseMove(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void mouseLeave(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void winSize(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
void command(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void render(HDC hDC);

void initWithFrame();
bool IsExtensionSupported( char* szTargetExtension ) ;
void createAndInitConsole();

#endif