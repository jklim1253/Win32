#pragma once

#include "Window.h"
#include "utility.hpp"
//#include "Grippable.h"

struct AboutImpl;
class AboutWindow : public Window {
public :
	AboutWindow();
	~AboutWindow();

	INT Initialize();

protected :
	void setEventHandler();
protected :
	LRESULT OnClose(HWND, WPARAM, LPARAM);
	LRESULT OnPaint(HWND, WPARAM, LPARAM);
	LRESULT OnTimer(HWND, WPARAM, LPARAM);

private :
	AboutImpl* pimpl;
};