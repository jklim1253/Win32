#pragma once

#include <Windows.h>

class Control {

protected :
	Control();
	virtual ~Control();

	virtual HWND Create(HWND hParentWnd, DWORD dwExStyle, DWORD dwStyle, RECT& rc);
	HWND GetHandle() const;
	void Show(int nCmdShow = SW_NORMAL);
	void Invalidate(BOOL bErase = TRUE);

protected :
	HWND hMainWnd;
};