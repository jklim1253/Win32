#pragma once

#include "Window.h"
//#include "Grippable.h"

class AboutWindow : public Window {
public :
	AboutWindow();

	INT Initialize();

protected :
	void setEventHandler();
protected :
	LRESULT OnClose(HWND, WPARAM, LPARAM);
	LRESULT OnPaint(HWND, WPARAM, LPARAM);
};