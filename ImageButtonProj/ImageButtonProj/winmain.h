#pragma once

#include <Windows.h>
#include <tchar.h>

#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

enum CONTROL_ID {
	ID_PLUS = 0,
	ID_MAXMIN,
	ID_CLOSE,
};

//enum USER_MESSAGE_ID {
//	BM_SETIMAGELIST = WM_USER + 1,
//};
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);