#pragma once

#include <Windows.h>

template<typename T>
class Singleton {
protected :
	Singleton() {}
	virtual ~Singleton() {}
public :
	static T& GetReference() {
		static T instance;
		return instance;
	}
};

template<typename GDIObject>
GDIObject Select(HDC hdc, GDIObject obj) {
	return (GDIObject)::SelectObject(hdc, obj);
}