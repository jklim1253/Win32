#pragma once
#include <Windows.h>
#include <tchar.h>
#include "type.h"

template<typename T>
class IControl {
public :
	void Invalidate(BOOL bErase = TRUE) {
		::InvalidateRect(hOwner, &Rect::clientRect(hOwner), bErase);
	}
	// rc : coordinate relative to left-top corner of parent window's client area.
	void Move(Rect rc, BOOL bRepaint = TRUE) {
		::MoveWindow(hOwner, rc.left, rc.top, rc.width(), rc.height(), bRepaint);
	}
	void Move(LONG left, LONG top, BOOL bRepaint = TRUE) {
		Rect rc = Rect::clientRect(hOwner);
		::MoveWindow(hOwner, left, top, rc.width(), rc.height(), bRepaint);
	}
	bool HitTest() {
		Rect rc = Rect::windowRect(hOwner);
		Point ptMouse;
		if (!::GetCursorPos(&ptMouse)) {
			DWORD dwError = ::GetLastError();
		}
		if (::PtInRect(&rc, ptMouse)) {
			return true;
		}
		return false;
	}
protected :
	unsigned long generateId() {
		return gid++;
	}
protected :
	HWND hOwner;

private :
	static unsigned long gid;
};

template<typename T>
unsigned long IControl<T>::gid = 0;