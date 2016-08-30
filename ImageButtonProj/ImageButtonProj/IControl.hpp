#pragma once
#include <Windows.h>
#include <tchar.h>
#include "type.h"
#include "IWindow.h"

template<typename T>
class IControl : public IWindow {
protected :
	IControl() : hParentWnd(NULL) {
	}
public :
	HWND GetParentHandle() const {
		return hParentWnd;
	}
protected :
	unsigned long generateId() {
		return gid++;
	}
protected :
	HWND hParentWnd;
private :
	static unsigned long gid;
};

template<typename T>
unsigned long IControl<T>::gid = 0;