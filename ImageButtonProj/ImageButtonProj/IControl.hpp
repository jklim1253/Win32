#pragma once
#include <Windows.h>
#include <tchar.h>
#include "type.h"
#include "IWindow.h"

template<typename T>
class IControl : public IWindow {
public :
protected :
	unsigned long generateId() {
		return gid++;
	}
private :
	static unsigned long gid;
};

template<typename T>
unsigned long IControl<T>::gid = 0;