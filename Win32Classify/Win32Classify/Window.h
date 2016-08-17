#pragma once

#include <windows.h>
#include <tchar.h>
#include <map>

class Window {
public :
	typedef LRESULT(Window::*Reaction)(HWND, WPARAM, LPARAM);
	typedef std::map<UINT, Reaction> EventMapType;
	typedef typename EventMapType::iterator EventIter;
public :
	Window();
	virtual ~Window();

public :
	LPCTSTR Register(LPCTSTR szClassName);
	HWND Create(HWND hParentWnd, DWORD dwExStyle, DWORD dwStyle, RECT& rc, LPCTSTR szClassName, LPCTSTR szTitle);
	HWND GetHwnd() const;
	void Show(int nCmdShow = SW_NORMAL);
	void Invalidate(BOOL bErase = TRUE);

protected :
	LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	EventMapType& getEventMap();
	virtual void setEventHandler() = 0;
	void AddEventHandler(UINT, const Reaction&);
protected :
	EventMapType EventMap;
	HWND hMainWnd;
};

