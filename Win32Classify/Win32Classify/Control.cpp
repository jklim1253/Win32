#include "Control.h"

Control::Control()
{
}

Control::~Control()
{
}

HWND Control::Create(HWND hParentWnd, DWORD dwExStyle, DWORD dwStyle, RECT & rc)
{
	return NULL;
}

HWND Control::GetHandle() const
{
	return hMainWnd;
}

void Control::Show(int nCmdShow)
{
	::ShowWindow(hMainWnd, nCmdShow);
}

void Control::Invalidate(BOOL bErase)
{
	RECT rc;
	::GetClientRect(hMainWnd, &rc);
	::InvalidateRect(hMainWnd, &rc, bErase);
}
