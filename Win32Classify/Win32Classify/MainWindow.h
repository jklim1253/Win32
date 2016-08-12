#pragma once

#include "Window.h"
//#include "Grippable.h"
#include "About.h"

class MainWindow : public Window {
public :
	MainWindow();

	INT Initialize();
	INT Run();

protected :
	void setEventHandler();
protected :
	LRESULT OnCreate(HWND, WPARAM, LPARAM);
	LRESULT OnDestroy(HWND, WPARAM, LPARAM);
	LRESULT OnPaint(HWND, WPARAM, LPARAM);
	LRESULT OnKeyDown(HWND, WPARAM, LPARAM);
	LRESULT OnLButtonDown(HWND, WPARAM, LPARAM);
	LRESULT OnLButtonUp(HWND, WPARAM, LPARAM);
	LRESULT OnMouseMove(HWND, WPARAM, LPARAM);

private :
	AboutWindow winAbout;

	BOOL bGripWindow;
	POINT ptMouse;
};