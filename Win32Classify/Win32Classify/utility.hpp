#pragma once

namespace Lim {
	template<typename GDIObject>
	GDIObject Select(HDC hdc, GDIObject obj) {
		return (GDIObject)::SelectObject(hdc, obj);
	}
	// free function
	template<typename pointer>
	void SafeDelete(pointer& p) {
		if (p) {
			delete p;
			p = nullptr;
		}
	}
}