#pragma once

#include <Windows.h>
#include <tchar.h>

class Grippable {
public :
	Grippable();

protected :
	LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
protected :
	BOOL bGripWindow;
	POINT ptMouse;
};
