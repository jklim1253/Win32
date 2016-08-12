#pragma once

#include <Windows.h>
#include <tchar.h>

class Grippable {
public :
	Grippable();

public :
	LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT OnLButtonDown(HWND, WPARAM, LPARAM);
	LRESULT OnLButtonUp(HWND, WPARAM, LPARAM);
	LRESULT OnMouseMove(HWND, WPARAM, LPARAM);
protected :
	BOOL bGripWindow;
	POINT ptMouse;
};
